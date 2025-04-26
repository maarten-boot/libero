#! /usr/bin/env python3

import sys
import logging
from libero_fsm_parser import LiberoFsmParser

logger = logging.Logger("")

if __name__ == "__main__":
    files = [
        # "t1.l",
        "src/Install/install.l",
        "src/lrcalcd.l",
        "src/lrcoded.l",
        "src/lrevald.l",
        "src/lrloadd.l",
        "pub/example/acms/testast.l",
        "pub/example/config/control.l",
        "pub/example/erbot/erbot.l",
        "pub/example/expr/calcpkd.l",
        "pub/example/expr/calculd.l",
        "pub/example/expr/exprcom.l",
        "pub/example/expr/exprexe.l",
        "pub/example/expr/exprrex.l",
        "pub/example/expr/exprsub.l",
        "pub/example/expr/vbcalc.l",
        "pub/example/htmlpp/htmlpp.l",
        "pub/example/install/install.l",
        "pub/example/picture/pctpictd.l",
        "pub/example/picture/ppictd.l",
        "pub/example/stripper/stripper.l",
        "pub/example/tcpip/cli101d.l",
    ]

    def xmain() -> None:
        for file in files:
            lfp = LiberoFsmParser(file=file)
            _ = lfp
            print(lfp.as_yaml())
            # print(lfp.options)
            sys.exit(0)

    xmain()
