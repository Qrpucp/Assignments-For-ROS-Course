
(cl:in-package :asdf)

(defsystem "off_board-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "camera" :depends-on ("_package_camera"))
    (:file "_package_camera" :depends-on ("_package"))
  ))