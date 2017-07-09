#! /usr/bin/env guile
!#

(set! %load-path (cons "guile" %load-path))
(set! %load-path (cons "../guix" %load-path))
(set! %load-compiled-path (cons "guile" %load-compiled-path))
(set! %load-compiled-path (cons "../guix" %load-compiled-path))

(use-modules (guix shell-utils))

;; FIXME: .go dependencies
;; workaround: always update .go before calculating hashes
;;(use-modules ((mes make) #:select (sytem**)))
(let* ((scm-files '("guix/make.scm"
                    "guix/records.scm"
                    "guix/shell-utils.scm"
                    "language/c99/compiler.scm"
                    "mes/as-i386.scm"
                    "mes/as.scm"
                    "mes/elf.scm"
                    "mes/M1.scm")))
  (setenv "srcdir" "guile")
  (setenv "host" %host-type)
  (with-directory-excursion "guile"
    (apply system* `("guile"
                     "--no-auto-compile"
                     "-L" "."
                     "-C" "."
                     "-s"
                     "../build-aux/compile-all.scm"
                     ,@scm-files))))

(use-modules (srfi srfi-1)
             (srfi srfi-26)
             (ice-9 curried-definitions)
             (ice-9 match)
             (guix make))

(add-target (bin.mescc "stage0/exit-42.c" #:libc #f))
(add-target (check "stage0/exit-42.0-guile" #:signal 11))  ; FIXME: segfault

(add-target (bin.mescc "stage0/exit-42.c" #:libc mini-libc-mes.E))
(add-target (check "stage0/exit-42.mini-guile" #:exit 42))

(add-target (bin.mescc "stage0/exit-42.c"))
(add-target (check "stage0/exit-42.guile" #:exit 42))

(define* (add-scaffold-test name #:key (exit 0) (libc libc-mes.E))
  (add-target (bin.gcc (string-append "scaffold/tests/" name ".c") #:libc #f))
  (add-target (check (string-append "scaffold/tests/" name ".mlibc-gcc") #:exit exit))

  (add-target (bin.mescc (string-append "scaffold/tests/" name ".c") #:libc libc))
  (add-target (check (string-append "scaffold/tests/" name "." (cond ((not libc) "0-")
                                                                     ((eq? libc mini-libc-mes.E) "mini-")
                                                                     (else "")) "guile") #:exit exit)))

;; tests/00: exit, functions without libc
(add-scaffold-test "00-exit-0" #:libc #f)
(add-scaffold-test "01-return-0" #:libc #f)
(add-scaffold-test "02-return-1" #:libc #f #:exit 1)
(add-scaffold-test "03-call" #:libc #f)
(add-scaffold-test "04-call-0" #:libc #f)
(add-scaffold-test "05-call-1" #:libc #f #:exit 1)
(add-scaffold-test "06-call-!1" #:libc #f)

(add-target (group "check-scaffold-tests/0" #:dependencies (filter (target-prefix? "check-scaffold/tests/0") %targets)))

;; tests/10: control without libc
(for-each
 (cut add-scaffold-test <> #:libc #f)
 '("10-if-0"
   "11-if-1"
   "12-if-=="
   "13-if-!="
   "14-if-goto"
   "15-if-!f"
   "16-if-t"))

(add-target (group "check-scaffold-tests/1" #:dependencies (filter (target-prefix? "check-scaffold/tests/1") %targets)))

;; tests/20: loop without libc
(for-each
 (cut add-scaffold-test <> #:libc #f)
 '("20-while"
   "21-char[]"
   "22-while-char[]"))

(add-target (group "check-scaffold-tests/2" #:dependencies (filter (target-prefix? "check-scaffold/tests/2") %targets)))

;; tests/30: call, compare: mini-libc-mes.c
(for-each
 (cut add-scaffold-test <> #:libc mini-libc-mes.E)
 '("30-strlen"
   "31-eputs"
   "32-compare"
   "33-and-or"
   "34-pre-post"
   "35-compare-char"
   "36-compare-arithmetic"
   "37-compare-assign"
   "38-compare-call"))

(add-target (group "check-scaffold-tests/3" #:dependencies (filter (target-prefix? "check-scaffold/tests/3") %targets)))

;; tests/40: control: mini-libc-mes.c
(for-each
 (cut add-scaffold-test <> #:libc mini-libc-mes.E)
 '("40-if-else"
   "41-?"
   "42-goto-label"
   "43-for-do-while"
   "44-switch"
   "45-void-call"))

(add-target (group "check-scaffold-tests/4" #:dependencies (filter (target-prefix? "check-scaffold/tests/4") %targets)))

;; tests/50: libc-mes.c
(for-each
 add-scaffold-test
 '("50-assert"
   "51-strcmp"
   "52-itoa"
   "54-argv"))

(add-target (group "check-scaffold-tests/5" #:dependencies (filter (target-prefix? "check-scaffold/tests/5") %targets)))

;; tests/60: building up to scaffold/m.c, scaffold/micro-mes.c
(for-each
 add-scaffold-test
 '("60-math"
   "61-array"
   "63-struct-cell"
   "64-make-cell"
   "65-read"))

(add-target (group "check-scaffold-tests/6" #:dependencies (filter (target-prefix? "check-scaffold/tests/6") %targets)))

;; tests/70: and beyond src/mes.c -- building up to 8cc.c, pcc.c, tcc.c, libguile/eval.c
(for-each
 add-scaffold-test
 '("70-printf"
   "71-struct-array"))

(add-target (group "check-scaffold-tests/7" #:dependencies (filter (target-prefix? "check-scaffold/tests/7") %targets)))

(add-target (group "check-scaffold-tests" #:dependencies (filter (target-prefix? "check-scaffold/tests") %targets)))

(add-target (group "check-scaffold" #:dependencies (filter (target-prefix? "check-scaffold") %targets)))

(add-target (bin.gcc "scaffold/hello.c"))
(add-target (check "scaffold/hello.gcc" #:exit 42))

(add-target (bin.gcc "scaffold/hello.c" #:libc #f))
(add-target (check "scaffold/hello.mlibc-gcc" #:exit 42))

(add-target (bin.mescc "scaffold/hello.c" #:libc mini-libc-mes.E))
(add-target (check "scaffold/hello.mini-guile" #:exit 42))

(add-target (bin.mescc "scaffold/hello.c"))
(add-target (check "scaffold/hello.guile" #:exit 42))


(add-target (bin.gcc "scaffold/m.c"))
(add-target (check "scaffold/m.gcc" #:exit 255))

(add-target (bin.gcc "scaffold/m.c" #:libc #f))
(add-target (check "scaffold/m.mlibc-gcc" #:exit 255))

(add-target (bin.mescc "scaffold/m.c"))
(add-target (check "scaffold/m.guile" #:exit 255))

(add-target (bin.gcc "scaffold/micro-mes.c" #:libc #f))
(add-target (check "scaffold/micro-mes.mlibc-gcc" #:exit 1))

(add-target (bin.mescc "scaffold/micro-mes.c"))
(add-target (check "scaffold/micro-mes.guile" #:exit 1))

(define snarf-bases
  '("gc" "lib" "math" "mes" "posix" "reader" "vector"))

(define bla
  `(,@(map (cut string-append "src/" <> ".c") snarf-bases)
    ,@(map (cut string-append "src/" <> ".mes.h") snarf-bases)
    ,@(map (cut string-append "src/" <> ".mes.i") snarf-bases)
    ,@(map (cut string-append "src/" <> ".mes.environment.i") snarf-bases)))

(define gcc-snarf-targets
  (list
   (add-target (snarf "src/gc.c" #:mes? #f))
   (add-target (snarf "src/lib.c" #:mes? #f))
   (add-target (snarf "src/math.c" #:mes? #f))
   (add-target (snarf "src/mes.c" #:mes? #f))
   (add-target (snarf "src/posix.c" #:mes? #f))
   (add-target (snarf "src/reader.c" #:mes? #f))
   (add-target (snarf "src/vector.c" #:mes? #f))))

(define mes-snarf-targets
  (list
   (add-target (snarf "src/gc.c" #:mes? #t))
   (add-target (snarf "src/lib.c" #:mes? #t))
   (add-target (snarf "src/math.c" #:mes? #t))
   (add-target (snarf "src/mes.c" #:mes? #t))
   (add-target (snarf "src/posix.c" #:mes? #t))
   (add-target (snarf "src/reader.c" #:mes? #t))
   (add-target (snarf "src/vector.c" #:mes? #t))))

(define VERSION "0.8")
(define PREFIX (or (getenv "PREFIX") "/usr/local"))
(define DATADIR (or (getenv "DATADIR") (string-append PREFIX " /share")))
(define MODULEDIR (or (getenv "MODULEDIR") (string-append DATADIR "/module/")))

(add-target (bin.gcc "src/mes.c" #:dependencies gcc-snarf-targets
                     #:defines `("FIXED_PRIMITIVES=1"
                                 "MES_FULL=1"
                                 "POSIX=1"
                                 ,(string-append "VERSION=\"" VERSION "\"")
                                 ,(string-append "MODULEDIR=\"" MODULEDIR "\"")
                                 ,(string-append "PREFIX=\"" PREFIX "\""))))

(add-target (bin.gcc "src/mes.c" #:libc #f
                     #:dependencies mes-snarf-targets
                     #:defines `("FIXED_PRIMITIVES=1"
                                 "MES_FULL=1"
                                 ,(string-append "VERSION=\"" VERSION "\"")
                                 ,(string-append "MODULEDIR=\"" MODULEDIR "\"")
                                 ,(string-append "PREFIX=\"" PREFIX "\""))))

(add-target (bin.mescc "src/mes.c" #:dependencies mes-snarf-targets
                       #:defines `("FIXED_PRIMITIVES=1"
                                   "MES_FULL=1"
                                 ,(string-append "VERSION=\"" VERSION "\"")
                                 ,(string-append "MODULEDIR=\"" MODULEDIR "\"")
                                 ,(string-append "PREFIX=\"" PREFIX "\""))))

(define mes-tests
  '("tests/read.test"
    "tests/base.test"
    "tests/closure.test"
    "tests/quasiquote.test"
    "tests/let.test"
    "tests/scm.test"
    "tests/display.test"
    "tests/cwv.test"
    "tests/math.test"
    "tests/vector.test"
    "tests/srfi-1.test"
    "tests/srfi-13.test"
    "tests/srfi-14.test"
    "tests/optargs.test"
    "tests/fluids.test"
    "tests/catch.test"
    "tests/psyntax.test"
    "tests/pmatch.test"
    "tests/let-syntax.test"
    "tests/guile.test"
    "tests/record.test"
    ;;sloooowwww
    ;;"tests/match.test"
    ;;"tests/peg.test"
    ))

(define (add-mes.gcc-test o)
  (add-target (target (file-name o)))
  (add-target (check o #:dependencies (list (get-target "src/mes.mlibc-gcc")))))

(define (add-mes.guile-test o)
  (add-target (target (file-name o)))
  (add-target (check o #:dependencies (list (get-target "src/mes.guile")))))

;; takes long, and should always pass if...
;;(for-each add-mes.gcc-test mes-tests)

;; ...mes.guile passes :-)
(for-each add-mes.guile-test mes-tests)

;; FIXME: run tests/base.test
(setenv "MES" "src/mes.guile")

(define (main args)
  (cond ((member "clean" args) (clean))
        ((member "help" args) (format #t "Usage: ./make.scm [TARGET]...

Targets:
    all
    check
    clean
    help~a
"
                                      ;;(string-join (map target-file-name %targets) "\n    " 'prefix)
                                      (string-join (filter (negate (cut string-index <> #\/)) (map target-file-name %targets)) "\n    " 'prefix)))
        (else
         (let ((targets (match args
                          (() (filter (negate check-target?) %targets))
                          ((? (cut member "all" <>)) (filter (negate check-target?) %targets))
                          ((? (cut member "check" <>)) (filter check-target? %targets))
                          (_ (filter-map (cut get-target <>) args)))))
           (for-each build targets)
           ;;((@@ (mes make) store) #:print 0)
           (exit %status)))))

(main (cdr (command-line)))
