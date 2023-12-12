;; Load the publishing system
(require 'ox-publish)
;; For information on publishing variables see
;; https://orgmode.org/manual/Publishing-options.html

;; Allow the website style to be obtained from github
(setq org-safe-remote-resources '("\\`https://fniessen\\.github\\.io\\(?:/\\|\\'\\)"))
;; Allow Minted to be executed
(setq org-latex-listings 'minted
      org-latex-packages-alist '(("" "minted"))
      org-latex-pdf-process
      '("pdflatex -shell-escape -interaction nonstopmode -output-directory %o %f"
        "pdflatex -shell-escape -interaction nonstopmode -output-directory %o %f"
        "pdflatex -shell-escape -interaction nonstopmode -output-directory %o %f"))

;; Define the publishing project
(setq org-publish-project-alist
      ;; Take org files and generate the html pages
      `(("html"
         :base-directory "./src/docs"
         :base-extension "org"
         :publishing-directory "./docs/"
         :publishing-function org-html-publish-to-html)
        ("png-windows"
         :base-directory "./src/docs/screenshots/Windows"
         :base-extension "png"
         :publishing-directory "./docs/screenshots/Windows"
         :publishing-function org-publish-attachment)
        ("png-macos"
         :base-directory "./src/docs/screenshots/macOS"
         :base-extension "png"
         :publishing-directory "./docs/screenshots/macOS"
         :publishing-function org-publish-attachment)
        ("pdf"
         :base-directory "./src/docs"
         :base-extension "org"
         :publishing-directory "./docs/"
         :publishing-function org-latex-publish-to-pdf)
        ("arbCoding.github.io/sac-format"
         :components ("html",
                      "png-windows",
                      "png-macos",
                      "pdf"))))

;; Generate the site output
(org-publish-all t)

;; We're done!
(message "Build complete!")
