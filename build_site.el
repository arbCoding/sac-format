;; Load the publishing system
(require 'ox-publish)
;; For information on publishing variables see
;; https://orgmode.org/manual/Publishing-options.html

(setq org-safe-remote-resources '("\\`https://fniessen\\.github\\.io\\(?:/\\|\\'\\)"))

;; Define the publishing project
(setq org-publish-project-alist
      ;; Take org files and generate the html pages
      `(("pages"
         :base-directory "./src/docs"
         :base-extension "org"
         :publishing-directory "./docs/"
         :publishing-function org-html-publish-to-html)
        ("AlexanderB.xyz" :components ("pages"))))

;; Generate the site output
(org-publish-all t)

;; We're done!
(message "Build complete!")
