;メニューバーの作成用関数
(define (menu-item name id)
  (if (and (string? name) (integer? id))
    (list name id)
    #f
  )
)

(define (menu-sub hmenu itemlist)
  (if (null? itemlist)
    0
    (let ((first (caar itemlist)) (second (cadar itemlist)))
      (if (string? first)
        (insertmenu hmenu 0 (+ MF-BYCOMMAND MF-STRING) second first)
        (insertmenu hmenu 0 (+ MF-BYCOMMAND MF-STRING MF-POPUP) first second)
      )
      (menu-sub hmenu (cdr itemlist))
    )
  )
)

(define (menu-popup popup-name . itemlist)
  (let
    (
      (hmenu (createpopupmenu))
    )
    (menu-sub hmenu itemlist)
    (list hmenu popup-name)
  )
)

(define (create-menu-bar . itemlist)
  (let
    (
      (hmenu (createmenu))
    )
    (menu-sub hmenu itemlist)
    hmenu
  )
)

;メニューIDの定義
(define idm-about 104)		;バージョン
(define idm-exit  105)		;アプリの終了
(define idm-console #x8000)	;コンソール表示
(define idm-user #x8001)	;ユーザーはこれ以降に定義

;メニューバーの生成
;描画ウインドウのメニューを換えたい場合はここの定義を変更
(define *handle-menu-bar*
  (create-menu-bar
    (menu-popup "ファイル(&F)"
      (menu-item "Wintyの終了(&X)" idm-exit)
    )
    (menu-popup "表\示(&V)"
      (menu-item "コンソ\ール(&T)" idm-console)
    )
    (menu-popup "ヘルプ(&H)"
      (menu-item "Version(&A)" idm-about)
    )
  )
)

(setmenu *handle-window* *handle-menu-bar*)	;メニューバーをウインドウに設定

;(sleep 1000)
;(load "demo.scm")
;(freeconsole)
;(allocconsole)
;(define (current-input-port) (console-input-port))
;(define (current-output-port) (console-output-port))
;(display "aaaaa")
;(sleep 5000)

;(freeconsole)
;(allocconsole)
;(set-input-port (console-input-port))
;(set-output-port (console-output-port))


(define (console)
  (freeconsole)
  (sleep 2000)
  (allocconsole)
  (set-input-port (console-input-port))
  (set-output-port (console-output-port))
)


