;���j���[�o�[�̍쐬�p�֐�
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

;���j���[ID�̒�`
(define idm-about 104)		;�o�[�W����
(define idm-exit  105)		;�A�v���̏I��
(define idm-console #x8000)	;�R���\�[���\��
(define idm-user #x8001)	;���[�U�[�͂���ȍ~�ɒ�`

;���j���[�o�[�̐���
;�`��E�C���h�E�̃��j���[�����������ꍇ�͂����̒�`��ύX
(define *handle-menu-bar*
  (create-menu-bar
    (menu-popup "�t�@�C��(&F)"
      (menu-item "Winty�̏I��(&X)" idm-exit)
    )
    (menu-popup "�\\��(&V)"
      (menu-item "�R���\\�[��(&T)" idm-console)
    )
    (menu-popup "�w���v(&H)"
      (menu-item "Version(&A)" idm-about)
    )
  )
)

(setmenu *handle-window* *handle-menu-bar*)	;���j���[�o�[���E�C���h�E�ɐݒ�

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


