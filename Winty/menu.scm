(load "winapi.scm")

(define loadmenu (lambda (hinstance lpmenuname) (call-library dlluser32 "LoadMenuA" hinstance lpmenuname)))
(define getwindowlong (lambda (handle-window param) (call-library dlluser32 "GetWindowLongA" handle-window param)))
(define setmenu (lambda (handle-window handle-menu) (call-library dlluser32 "SetMenu" handle-window handle-menu)))
(define getmenu (lambda (handle-window) (call-library dlluser32 "GetMenu" handle-window)))
(define getinstance (lambda (handle-window) (getwindowlong handle-window GWL-HINSTANCE)))	;インスタンスの取得
(define drawmenubar (lambda (handle-window) (call-library dlluser32 "DrawMenuBar" handle-window)))
(define createmenu (lambda () (call-library dlluser32 "CreateMenu")))
(define insertmenu (lambda (handle-menu position flag item-or-hmenu newitem) (call-library dlluser32 "InsertMenuA" handle-menu position flag itemtype newitem)))
(define createpopupmenu (lambda () (call-library dlluser32 "CreatePopupMenu")))

;ex
;(define hmenu (getmenu hwnd))
;(insertmenu hmenu 0 (+ MF-BYCOMMAND MF-STRING) 666 "inserttest")
;(insertmenu hmenu 32773 (+ MF-BYCOMMAND MF-STRING) 666 "inserttest(&D)")
;(insertmenu hmenu 666 (+ MF-BYCOMMAND MF-STRING) 667 "i667(&Y)")
;(drawmenubar)


; * Window field offsets for GetWindowLong()
;まだたくさんあるけどとりあえず
(define GWL-WNDPROC -4)
(define GWL-HINSTANCE -6)
(define GWL-HWNDPARENT -8)
(define GWL-STYLE -16)
(define GWL-EXSTYLE -20)
(define GWL-USERDATA -21)
(define GWL-ID -12)

;define insertMenu parameter
;/* ;win40  -- A lot of MF_* flags have been renamed as MFT_* and MFS_* flags */
;/*
; * Menu flags for Add/Check/EnableMenuItem()
; */
(define MF-INSERT           #x00000000)
(define MF-CHANGE           #x00000080)
(define MF-APPEND           #x00000100)
(define MF-DELETE           #x00000200)
(define MF-REMOVE           #x00001000)

(define MF-BYCOMMAND        #x00000000)
(define MF-BYPOSITION       #x00000400)

(define MF-SEPARATOR        #x00000800)

(define MF-ENABLED          #x00000000)
(define MF-GRAYED           #x00000001)
(define MF-DISABLED         #x00000002)

(define MF-UNCHECKED        #x00000000)
(define MF-CHECKED          #x00000008)
(define MF-USECHECKBITMAPS  #x00000200)

(define MF-STRING           #x00000000)
(define MF-BITMAP           #x00000004)
(define MF-OWNERDRAW        #x00000100)

(define MF-POPUP            #x00000010)
(define MF-MENUBARBREAK     #x00000020)
(define MF-MENUBREAK        #x00000040)

(define MF-UNHILITE         #x00000000)
(define MF-HILITE           #x00000080)

(define MF-DEFAULT          #x00001000)
(define MF-SYSMENU          #x00002000)
(define MF-HELP             #x00004000)
(define MF-RIGHTJUSTIFY     #x00004000)

(define MF-MOUSESELECT      #x00008000)		;;/* Obsolete -- only used by old RES files */
(define MF-END              #x00000080)


(define MFT-STRING          MF-STRING)
(define MFT-BITMAP          MF-BITMAP)
(define MFT-MENUBARBREAK    MF-MENUBARBREAK)
(define MFT-MENUBREAK       MF-MENUBREAK)
(define MFT-OWNERDRAW       MF-OWNERDRAW)
(define MFT-RADIOCHECK      #x00000200)
(define MFT-SEPARATOR       MF-SEPARATOR)
(define MFT-RIGHTORDER      #x00002000)
(define MFT-RIGHTJUSTIFY    MF-RIGHTJUSTIFY)

;;/* Menu flags for Add/Check/EnableMenuItem() */
(define MFS-GRAYED          #x00000003)
(define MFS-DISABLED        MFS-GRAYED)
(define MFS-CHECKED         MF-CHECKED)
(define MFS-HILITE          MF-HILITE)
(define MFS-ENABLED         MF-ENABLED)
(define MFS-UNCHECKED       MF-UNCHECKED)
(define MFS-UNHILITE        MF-UNHILITE)
(define MFS-DEFAULT         MF-DEFAULT)


