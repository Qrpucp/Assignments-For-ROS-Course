;; Auto-generated. Do not edit!


(when (boundp 'off_board::camera)
  (if (not (find-package "OFF_BOARD"))
    (make-package "OFF_BOARD"))
  (shadow 'camera (find-package "OFF_BOARD")))
(unless (find-package "OFF_BOARD::CAMERA")
  (make-package "OFF_BOARD::CAMERA"))

(in-package "ROS")
;;//! \htmlinclude camera.msg.html


(defclass off_board::camera
  :super ros::object
  :slots (_find_obs_flag _dis _pos_left _pos_right _x_pos _y_pos ))

(defmethod off_board::camera
  (:init
   (&key
    ((:find_obs_flag __find_obs_flag) nil)
    ((:dis __dis) 0.0)
    ((:pos_left __pos_left) 0.0)
    ((:pos_right __pos_right) 0.0)
    ((:x_pos __x_pos) 0.0)
    ((:y_pos __y_pos) 0.0)
    )
   (send-super :init)
   (setq _find_obs_flag __find_obs_flag)
   (setq _dis (float __dis))
   (setq _pos_left (float __pos_left))
   (setq _pos_right (float __pos_right))
   (setq _x_pos (float __x_pos))
   (setq _y_pos (float __y_pos))
   self)
  (:find_obs_flag
   (&optional __find_obs_flag)
   (if __find_obs_flag (setq _find_obs_flag __find_obs_flag)) _find_obs_flag)
  (:dis
   (&optional __dis)
   (if __dis (setq _dis __dis)) _dis)
  (:pos_left
   (&optional __pos_left)
   (if __pos_left (setq _pos_left __pos_left)) _pos_left)
  (:pos_right
   (&optional __pos_right)
   (if __pos_right (setq _pos_right __pos_right)) _pos_right)
  (:x_pos
   (&optional __x_pos)
   (if __x_pos (setq _x_pos __x_pos)) _x_pos)
  (:y_pos
   (&optional __y_pos)
   (if __y_pos (setq _y_pos __y_pos)) _y_pos)
  (:serialization-length
   ()
   (+
    ;; bool _find_obs_flag
    1
    ;; float32 _dis
    4
    ;; float32 _pos_left
    4
    ;; float32 _pos_right
    4
    ;; float32 _x_pos
    4
    ;; float32 _y_pos
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _find_obs_flag
       (if _find_obs_flag (write-byte -1 s) (write-byte 0 s))
     ;; float32 _dis
       (sys::poke _dis (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_left
       (sys::poke _pos_left (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _pos_right
       (sys::poke _pos_right (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _x_pos
       (sys::poke _x_pos (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _y_pos
       (sys::poke _y_pos (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _find_obs_flag
     (setq _find_obs_flag (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; float32 _dis
     (setq _dis (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_left
     (setq _pos_left (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _pos_right
     (setq _pos_right (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _x_pos
     (setq _x_pos (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y_pos
     (setq _y_pos (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get off_board::camera :md5sum-) "db349a97975dc8574cf482cb05513bb9")
(setf (get off_board::camera :datatype-) "off_board/camera")
(setf (get off_board::camera :definition-)
      "bool find_obs_flag
float32 dis
float32 pos_left
float32 pos_right
float32 x_pos
float32 y_pos
")



(provide :off_board/camera "db349a97975dc8574cf482cb05513bb9")


