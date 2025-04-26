!
!   myprogd.l   Dialog description for myprog.c
!
!   Written:    yy/mm/dd  My name
!   Revised:    yy/mm/dd
!
-source=myprog

After-Init:
    (--) Ok                         -> Next-State-Name
          +
    (--) Error                      ->
          + Terminate-The-Program

Defaults:
    (--) Exception                  ->
          + Terminate-The-Program

Next-State-Name: <aaa> <bbbb><ccc>
    (--) Event-Name                 -> Next-State-Name
          + Module-To-Execute
          + Module-To-Execute

Next-Wrong:
    (--) Event-Name                 -> xfter-Init
          + Module-To-Execute
          + Module-To-Execute
