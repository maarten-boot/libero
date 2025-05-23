#----------------------------------------------------------------------------
#   Dialog data and code block - do not modify this code by hand
#   Generated by LIBERO 2.30 on 26 Jun, 1998, 16:19.
#   Schema file used: lrschema.pl
#----------------------------------------------------------------------------

#-- Symbolic constants and event numbers ------------------------------------

$LR_defaults_state = 9;
$LR_STATE_after_init = 0;
$LR_STATE_have_argument = 1;
$LR_STATE_first_pass = 2;
$LR_STATE_first_pass_block = 3;
$LR_STATE_main_pass = 4;
$LR_STATE_main_pass_block = 5;
$LR_STATE_inter_pass = 6;
$LR_STATE_inter_pass_block = 7;
$LR_STATE_final_pass = 8;
$LR_STATE_defaults = 9;

$terminate_event = -1;                  # Halts the dialog
$LR_NULL_EVENT = -2;
$blank_line_event = 0;
$block_event = 1;
$body_text_event = 2;
$build_event = 3;
$comment_event = 4;
$define_event = 5;
$echo_event = 6;
$else_event = 7;
$end_block_event = 8;
$end_for_event = 9;
$end_if_event = 10;
$end_pipe_event = 11;
$error_event = 12;
$exception_event = 13;
$finished_event = 14;
$finished_include_event = 15;
$for_event = 16;
$if_event = 17;
$ignore_event = 18;
$ignore_pages_event = 19;
$include_event = 20;
$macro_event = 21;
$macro_text_event = 22;
$need_inter_pass_event = 23;
$ok_event = 24;
$page_event = 25;
$pipe_event = 26;
$switch_event = 27;

#-- Standard subroutines used by dialog interpreter -------------------------

sub raise_exception {
    local ($event) = @_;
    $the_exception_event = $event;
    $exception_raised = 1;
}

sub LR_initialise {
    $LR_nextst = "";
    $LR_nextst .= "000000000000000000000000000000000000000000";
    $LR_nextst .= "00000001000000";
    $LR_nextst .= "000000000000000000000000000001000000000000";
    $LR_nextst .= "00000002000001";
    $LR_nextst .= "020302020202020202020202020004020202020202";
    $LR_nextst .= "02020000020200";
    $LR_nextst .= "030303030303030302030303020304020303030303";
    $LR_nextst .= "03030303030303";
    $LR_nextst .= "040504040404040404040404040008040404040404";
    $LR_nextst .= "04040600040400";
    $LR_nextst .= "050505050505050504050505040508040505050505";
    $LR_nextst .= "05050605050505";
    $LR_nextst .= "060706060606060606060606060008060606060606";
    $LR_nextst .= "06060600060600";
    $LR_nextst .= "070707070707070706070707060708060707070707";
    $LR_nextst .= "07070607070707";
    $LR_nextst .= "080808080808080808080808080801080808080808";
    $LR_nextst .= "08080808080808";
    $LR_nextst .= "000000000000000000000000000900000000000000";
    $LR_nextst .= "00000000000000";

    $LR_action = "";
    $LR_action .= "000000000000000000000000020000000000000000";
    $LR_action .= "00000001000000";
    $LR_action .= "000000000000000000000000000005000000000000";
    $LR_action .= "00000003000004";
    $LR_action .= "081606230910241917222023090025092118131512";
    $LR_action .= "11070000142300";
    $LR_action .= "081608230923232323232323092325092323232323";
    $LR_action .= "23082323232323";
    $LR_action .= "083026310910241917222023090032092118281512";
    $LR_action .= "11273300292300";
    $LR_action .= "343034360936363609363636093632093636363636";
    $LR_action .= "36343536363636";
    $LR_action .= "083037310910241917222023090040092118281512";
    $LR_action .= "11383300392300";
    $LR_action .= "343034360936363609363636093640093636363636";
    $LR_action .= "36343336363636";
    $LR_action .= "424741474709474747474745094746474747474747";
    $LR_action .= "47414747434447";
    $LR_action .= "000000000000000000000000000200000000000000";
    $LR_action .= "00000000000000";

    $LR_module = "";
    $LR_module .= "222319" . "00";
    $LR_module .= "51" . "00";
    $LR_module .= "5245242620" . "00";
    $LR_module .= "3419" . "00";
    $LR_module .= "3751" . "00";
    $LR_module .= "17121520" . "00";
    $LR_module .= "18121520" . "00";
    $LR_module .= "1520" . "00";
    $LR_module .= "20" . "00";
    $LR_module .= "491320" . "00";
    $LR_module .= "4820" . "00";
    $LR_module .= "17502820" . "00";
    $LR_module .= "17211320" . "00";
    $LR_module .= "1705321320" . "00";
    $LR_module .= "3320" . "00";
    $LR_module .= "171320" . "00";
    $LR_module .= "4020" . "00";
    $LR_module .= "174320" . "00";
    $LR_module .= "4220" . "00";
    $LR_module .= "0920" . "00";
    $LR_module .= "173520" . "00";
    $LR_module .= "4120" . "00";
    $LR_module .= "1320" . "00";
    $LR_module .= "171620" . "00";
    $LR_module .= "1147362620" . "00";
    $LR_module .= "17031520" . "00";
    $LR_module .= "18031520" . "00";
    $LR_module .= "172120" . "00";
    $LR_module .= "05291731133020" . "00";
    $LR_module .= "0820" . "00";
    $LR_module .= "170420" . "00";
    $LR_module .= "112906443620" . "00";
    $LR_module .= "46362620" . "00";
    $LR_module .= "0220" . "00";
    $LR_module .= "47362620" . "00";
    $LR_module .= "0120" . "00";
    $LR_module .= "171520" . "00";
    $LR_module .= "181520" . "00";
    $LR_module .= "05311320" . "00";
    $LR_module .= "1106443620" . "00";
    $LR_module .= "171420" . "00";
    $LR_module .= "1420" . "00";
    $LR_module .= "17312520" . "00";
    $LR_module .= "2720" . "00";
    $LR_module .= "1020" . "00";
    $LR_module .= "11073819" . "00";
    $LR_module .= "3920" . "00";

    $LR_offset [1] = 0;
    $LR_offset [2] = 4;
    $LR_offset [3] = 6;
    $LR_offset [4] = 12;
    $LR_offset [5] = 15;
    $LR_offset [6] = 18;
    $LR_offset [7] = 23;
    $LR_offset [8] = 28;
    $LR_offset [9] = 31;
    $LR_offset [10] = 33;
    $LR_offset [11] = 37;
    $LR_offset [12] = 40;
    $LR_offset [13] = 45;
    $LR_offset [14] = 50;
    $LR_offset [15] = 56;
    $LR_offset [16] = 59;
    $LR_offset [17] = 63;
    $LR_offset [18] = 66;
    $LR_offset [19] = 70;
    $LR_offset [20] = 73;
    $LR_offset [21] = 76;
    $LR_offset [22] = 80;
    $LR_offset [23] = 83;
    $LR_offset [24] = 86;
    $LR_offset [25] = 90;
    $LR_offset [26] = 96;
    $LR_offset [27] = 101;
    $LR_offset [28] = 106;
    $LR_offset [29] = 110;
    $LR_offset [30] = 118;
    $LR_offset [31] = 121;
    $LR_offset [32] = 125;
    $LR_offset [33] = 132;
    $LR_offset [34] = 137;
    $LR_offset [35] = 140;
    $LR_offset [36] = 145;
    $LR_offset [37] = 148;
    $LR_offset [38] = 152;
    $LR_offset [39] = 156;
    $LR_offset [40] = 161;
    $LR_offset [41] = 167;
    $LR_offset [42] = 171;
    $LR_offset [43] = 174;
    $LR_offset [44] = 179;
    $LR_offset [45] = 182;
    $LR_offset [46] = 185;
    $LR_offset [47] = 190;
}

#-- Dialog interpreter starts here ------------------------------------------

$feedback = 0;                          #   Main program feedback
$LR_state = 0;                          #   First state is always 0
&LR_initialise;                         #   Initialise static strings
&initialise_the_program;
while ($the_next_event != $terminate_event)
  {
    $LR_event = $the_next_event;
    $LR_event < 0 || $LR_event >= 28 && die
        "State ", $LR_state,
        " - event ", $LR_event, " is out of range\n";
    $LR_index = ($LR_state * 28 + $LR_event) * 2;
    $LR_index = substr ($LR_action, $LR_index, 2);
    $LR_savest = $LR_state;

    #   If no action for this event, try the defaults state
    if ($LR_index == 0)
      {
        $LR_state = $LR_defaults_state;
        $LR_index = ($LR_state * 28 + $LR_event) * 2;
        $LR_index = substr ($LR_action, $LR_index, 2);
        $LR_index == 0 && die
            "State ", $LR_state,
            " - event ", $LR_event, " is not accepted\n";
      }
    $the_next_event = $LR_NULL_EVENT;
    $the_exception_event = $LR_NULL_EVENT;
    $exception_raised = 0;

    #   Execute module list for state transition, 0 indicates end of list
    $LR_vecptr = $LR_offset [$LR_index];
    $LR_modnbr = substr ($LR_module, $LR_vecptr * 2, 2);
    while ($LR_modnbr > 0 && !$exception_raised)
      {
          {                             #   Execute the module
            $LR_modnbr == 1 && do { &add_action_line_to_block; last; };
            $LR_modnbr == 2 && do { &add_line_to_block; last; };
            $LR_modnbr == 3 && do { &anchor_title_if_present; last; };
            $LR_modnbr == 4 && do { &build_specified_table; last; };
            $LR_modnbr == 5 && do { &check_if_ignore_pages; last; };
            $LR_modnbr == 6 && do { &check_if_inter_pass_needed; last; };
            $LR_modnbr == 7 && do { &clean_up_work_files; last; };
            $LR_modnbr == 8 && do { &clear_specified_block; last; };
            $LR_modnbr == 9 && do { &close_if_block_if_open; last; };
            $LR_modnbr == 10 && do { &close_piped_output; last; };
            $LR_modnbr == 11 && do { &close_the_document; last; };
            $LR_modnbr == 12 && do { &collect_title_if_present; last; };
            $LR_modnbr == 13 && do { &copy_action_line_to_output; last; };
            $LR_modnbr == 14 && do { &copy_line_to_final_output; last; };
            $LR_modnbr == 15 && do { &copy_line_to_output; last; };
            $LR_modnbr == 16 && do { &echo_text_to_console; last; };
            $LR_modnbr == 17 && do { &expand_symbols_in_line; last; };
            $LR_modnbr == 18 && do { &expand_symbols_in_macro; last; };
            $LR_modnbr == 19 && do { &get_next_argument_value; last; };
            $LR_modnbr == 20 && do { &get_next_document_line; last; };
            $LR_modnbr == 21 && do { &handle_ignore_command; last; };
            $LR_modnbr == 22 && do { &initialise_program_data; last; };
            $LR_modnbr == 23 && do { &load_anchor_definitions; last; };
            $LR_modnbr == 24 && do { &open_main_document; last; };
            $LR_modnbr == 25 && do { &open_new_output_page; last; };
            $LR_modnbr == 26 && do { &open_output_work_file; last; };
            $LR_modnbr == 27 && do { &open_piped_output; last; };
            $LR_modnbr == 28 && do { &open_the_document; last; };
            $LR_modnbr == 29 && do { &output_footer_for_page; last; };
            $LR_modnbr == 30 && do { &output_header_for_page; last; };
            $LR_modnbr == 31 && do { &parse_page_title_and_name; last; };
            $LR_modnbr == 32 && do { &parse_page_title_for_toc; last; };
            $LR_modnbr == 33 && do { &parse_page_title_only; last; };
            $LR_modnbr == 34 && do { &process_the_switch; last; };
            $LR_modnbr == 35 && do { &repeat_for_loop_contents; last; };
            $LR_modnbr == 36 && do { &reuse_work_file_as_input; last; };
            $LR_modnbr == 37 && do { &save_anchor_definitions; last; };
            $LR_modnbr == 38 && do { &signal_document_processed; last; };
            $LR_modnbr == 39 && do { &signal_internal_failure; last; };
            $LR_modnbr == 40 && do { &signal_unexpected_end_block; last; };
            $LR_modnbr == 41 && do { &signal_unexpected_end_for; last; };
            $LR_modnbr == 42 && do { &skip_else_block_always; last; };
            $LR_modnbr == 43 && do { &skip_if_block_if_false; last; };
            $LR_modnbr == 44 && do { &start_final_pass; last; };
            $LR_modnbr == 45 && do { &start_first_pass; last; };
            $LR_modnbr == 46 && do { &start_inter_pass; last; };
            $LR_modnbr == 47 && do { &start_main_pass; last; };
            $LR_modnbr == 48 && do { &store_macro_definition; last; };
            $LR_modnbr == 49 && do { &store_symbol_definition; last; };
            $LR_modnbr == 50 && do { &take_include_file_name; last; };
            $LR_modnbr == 51 && do { &terminate_the_program; last; };
            $LR_modnbr == 52 && do { &text_to_htmlpp_if_guru; last; };
          }
        $LR_vecptr++;
        $LR_modnbr = substr ($LR_module, $LR_vecptr * 2, 2);
      }

    #   Handle exception if any was raised
    if ($exception_raised)
      {
        $the_exception_event == $LR_NULL_EVENT ||
            ($LR_event = $the_exception_event);

        $the_next_event = $LR_event;
      }
    else
      {
        $LR_index = ($LR_state * 28 + $LR_event) * 2;
        $LR_state = substr ($LR_nextst, $LR_index, 2);
      }
    $LR_state = $LR_savest if ($LR_state == $LR_defaults_state);
    if ($the_next_event == $LR_NULL_EVENT)
      {
        &get_external_event;
        $the_next_event == $LR_NULL_EVENT && die
            "No event set after event ", $LR_event,
            " in state ", $LR_state, "\n";
      }
  }
exit ($feedback);
