#! /usr/bin/env python3

from typing import (
    Dict,
    List,
    Any,
)

# import sys
import re
import json
import yaml
import logging

logger = logging.Logger(__name__)

# Actions are also called Module in the libero doc's


class LiberoFsmParser:
    lines: List[str]
    line: str
    result: Dict[str, Any] = {}
    options: Dict[str, Any] = {}

    first_state: str
    current_state: str | None = None
    current_event: str | None = None
    current_action: str | None = None
    next_state: str | None = None
    super_state_list: List[str] = []

    identifier = r"(\w+[-\w]+)"

    # from result the action list may not be sorted
    # the target state cannot be the entry state: TODO

    def __init__(
        self,
        file: str,
    ) -> None:
        logger.debug("")

        self.file = file
        self.first_state = ""
        with open(self.file, "r", encoding="utf-8") as in_file:
            data = in_file.read()
            self.lines = data.splitlines()
            self._one_file()
            self.lines = []
            self.line = ""

    def _update_result(
        self,
    ) -> None:
        logger.debug("")

        if self.current_state and self.current_event and self.next_state and self.current_action:
            if self.current_state not in self.result:
                self.result[self.current_state] = {
                    "super": [],
                    "events": {},
                }

            if len(self.super_state_list):
                self.result[self.current_state]["super"] = self.super_state_list

            cs = self.result[self.current_state]["events"]
            if self.current_event not in cs:
                cs[self.current_event] = {
                    "next_state": self.next_state,
                    "action_list": [],
                }

            ce = self.result[self.current_state]["events"][self.current_event]
            ce["action_list"].append(self.current_action)

    def _ignore_comment_and_empty_lines(self) -> bool:
        logger.debug("")

        if len(self.line) == 0:  # ignore empty lines
            return True

        if self.line.startswith("!"):  # ignore comments
            return True

        if len(self.line) == 1:  # remove ^Z (SUB)
            return True

        return False

    def _do_options(self) -> bool:
        logger.debug("")

        while True:
            m = re.match(
                r"^[/-]((opt:)?(\w+))(=[^\s$]+)?",
                self.line,
            )  # Option, can be more on one self.line
            if not m:
                return False

            self.options[m[1]] = m[0]  # store the option found

            # clean the line and try again if not empty or comment
            self.line = self.line[len(m[0]) :].strip()
            if len(self.line) == 0:
                return True
            if self.line[0] == "!":
                return True

            # print("Option:", m[1], "##", self.line)

    def _do_action(self) -> bool:
        logger.debug("")

        m = re.match(
            r"\+\s*" + self.identifier,
            self.line,
        )  # Action
        if m:  # Action
            self.current_action = m[1]
            self._update_result()
            # print("Action:", m[1])
            return True
        return False

    def _do_event(self) -> bool:
        logger.debug("")

        # it seems event numbers were a event from the past and not used anymore
        m = re.match(
            r"^\([^\)]+\)\s*" + self.identifier + r"\s*->\s*" + self.identifier + "?",
            self.line,
        )
        if m:
            n = None
            if m[2] and m[1] != m[2]:
                n = m[2]
            else:
                n = "__SELF__"

            self.current_action = None
            self.current_event = m[1]
            self.next_state = n

            # print("Event:", m[1], n)
            return True

        m = re.match(
            r"^\(--\)\s*" + "(\\$other)" + r"\s*->\s*" + self.identifier + "?",
            self.line,
        )  # Event no number with $other
        if m:
            n = None
            if m[2] and m[1] != m[2]:
                n = m[2]
            else:
                n = "__SELF__"

            self.current_action = None
            self.current_event = m[1]
            self.next_state = n

            # print("Event:", m[1], n)
            return True

        return False

    def _get_super_states(self) -> None:
        logger.debug("")

        while True:
            if len(self.line) == 0:
                return
            if self.line[0] == "!":
                return

            assert self.line[0] == "<"

            m = re.match("<" + self.identifier + ">", self.line)  # a new state
            if m:
                _super = m[1]
                self.super_state_list.append(_super)
                self.line = self.line[len(m[0]) :].strip()

    def _do_state(self) -> bool:
        logger.debug("")

        m = re.match(self.identifier + ":", self.line)  # a new state
        if m:
            self.super_state_list = []
            self.lnext_state = None
            self.current_event = None
            self.current_action = None

            self.current_state = m[1]
            if not self.first_state:
                self.first_state = self.current_state

            self.line = self.line[len(m[0]) :].strip()
            self._get_super_states()

            return True

        return False

    def _one_file(
        self,
    ) -> None:
        logger.debug("")

        n = 0
        for self.line in self.lines:
            n += 1
            self.line = self.line.strip()
            if self._ignore_comment_and_empty_lines():
                continue

            if self._do_options():
                continue

            if self._do_action():
                continue

            if self._do_event():
                continue

            if self._do_state():
                continue

            if len(self.line):
                msg = f"Fatal: Unrecognized line type: line {n} has: {self.line}"
                raise Exception(msg)

    def _check_state(self) -> bool:  # noqa: C901
        # STATE
        # 1 all next states should exist as state except __SUPER__: done
        # 2 any next state may not be the first state: done
        # 3 any super state must extist as state: done
        # 4 any state that is not a next state is never reached (except the first-state): done
        #
        logger.debug("")

        r = False
        next_state_list: List[str] = []
        for state, state_dict in self.result.items():
            sup = state_dict["super"]
            if len(sup):
                for super_name in sup:
                    if super_name not in self.result:
                        r = True
                        msg = f"super state {super_name} " + f"used in {state} " + "is not defined"
                        logger.error(msg)
                        # print(msg, file=sys.stderr)

            for event, event_def in state_dict["events"].items():
                next_state = event_def["next_state"]
                if next_state in ["__SELF__"]:
                    continue

                if next_state not in next_state_list:
                    next_state_list.append(next_state)

                if next_state == self.first_state:
                    r = True
                    msg = (
                        f"next state {next_state} "
                        + f"cannot be first_state {self.first_state} "
                        + f"used in {state},{event}"
                    )
                    logger.error(msg)
                    # print(msg, file=sys.stderr)

                if next_state not in self.result:
                    r = True
                    msg = f"next state {next_state} " + f"used in {state},{event} " + " is not defined"
                    logger.error(msg)
                    # print(msg, file=sys.stderr)

        for state in self.result.keys():
            if state in ["After-Init", "Defaults"]:
                continue
            if state not in next_state_list:
                msg = f"state unreachable: {state} is never used as a next_state"
                logger.warning(msg)
                # print(msg, file=sys.stderr)

        return r

    def _validate(self) -> bool:
        # ACTION Validation
        # warn about action names that:
        # 1 repeat in the same action list.
        # 2 are not reached because the state+event is never reached.
        logger.debug("")

        # Todo: explode super states starting from the top
        # Populate the target state with all super events,
        # takining into account that identical named events lower in the chain,
        # overwrite events earlier specified.

        r = self._check_state()
        return r

    # PUBLIC
    def make_result(self) -> Dict[str, Any]:
        logger.debug("")
        has_error = self._validate()
        if has_error:
            return {}

        return {
            "file": self.file,
            "options": self.options,
            "first_state": self.first_state,
            "libero_fsm": self.result,
        }

    def as_json(self, indent: int = 4) -> str:
        logger.debug("")
        return json.dumps(
            self.make_result(),
            indent=indent,
            sort_keys=False,
        )

    def as_yaml(self, indent: int = 4) -> str:
        logger.debug("")
        return yaml.dump(
            self.make_result(),
            indent=indent,
            sort_keys=False,
        )

    # TODO: as xml (compat with https://github.com/zeromq/gsl )
