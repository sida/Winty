(load "winapi.scm")

(textout *handle-device* 10 10 "ÉfÉÇÉvÉçÉOÉâÉÄ" 14)

(define line 
  (lambda (x1 y1 x2 y2) 
    (begin
      (moveto *handle-device* x1 y1)
      (lineto *handle-device* x2 y2)
    )
  )
)

(define box
  (lambda (x1 y1 x2 y2)
    (begin
      (moveto *handle-device* x1 y1)
      (lineto *handle-device* x1 y2)
      (lineto *handle-device* x2 y2)
      (lineto *handle-device* x2 y1)
      (lineto *handle-device* x1 y1)
    )
  )
)


(define (sample x y size dx dy n)
  (if (= n 1)
    1
    (begin
      (box x y (+ x size) (+ y size))
      (sample (+ x dx) (+ y dy) size dx dy (- n 1))
    )
  )
)

(define (demo)
  (begin
    (sample 100 100 50 10 20 20)
    (flush-window *handle-window*)
  )
)

(demo)

