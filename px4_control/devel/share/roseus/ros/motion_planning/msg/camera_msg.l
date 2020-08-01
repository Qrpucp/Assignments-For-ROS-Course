;; Auto-generated. Do not edit!


(when (boundp 'motion_planning::camera_msg)
  (if (not (find-package "MOTION_PLANNING"))
    (make-package "MOTION_PLANNING"))
  (shadow 'camera_msg (find-package "MOTION_PLANNING")))
(unless (find-package "MOTION_PLANNING::CAMERA_MSG")
  (make-package "MOTION_PLANNING::CAMERA_MSG"))

(in-package "ROS")
;;//! \htmlinclude camera_msg.msg.html


(defclass motion_planning::camera_msg
  :super ros::object
  :slots (_find_box_flag _x_center_dis _y_center_dis _box_hight ))

(defmethod motion_planning::camera_msg
  (:init
   (&key
    ((:find_box_flag __find_box_flag) nil)
    ((:x_center_dis __x_center_dis) 0.0)
    ((:y_center_dis __y_center_dis) 0.0)
    ((:box_hight __box_hight) 0.0)
    )
   (send-super :init)
   (setq _find_box_flag __find_box_flag)
   (setq _x_center_dis (float __x_center_dis))
   (setq _y_center_dis (float __y_center_dis))
   (setq _box_hight (float __box_hight))
   self)
  (:find_box_flag
   (&optional __find_box_flag)
   (if __find_box_flag (setq _find_box_flag __find_box_flag)) _find_box_flag)
  (:x_center_dis
   (&optional __x_center_dis)
   (if __x_center_dis (setq _x_center_dis __x_center_dis)) _x_center_dis)
  (:y_center_dis
   (&optional __y_center_dis)
   (if __y_center_dis (setq _y_center_dis __y_center_dis)) _y_center_dis)
  (:box_hight
   (&optional __box_hight)
   (if __box_hight (setq _box_hight __box_hight)) _box_hight)
  (:serialization-length
   ()
   (+
    ;; bool _find_box_flag
    1
    ;; float64 _x_center_dis
    8
    ;; float64 _y_center_dis
    8
    ;; float64 _box_hight
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _find_box_flag
       (if _find_box_flag (write-byte -1 s) (write-byte 0 s))
     ;; float64 _x_center_dis
       (sys::poke _x_center_dis (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _y_center_dis
       (sys::poke _y_center_dis (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _box_hight
       (sys::poke _box_hight (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _find_box_flag
     (setq _find_box_flag (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; float64 _x_center_dis
     (setq _x_center_dis (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _y_center_dis
     (setq _y_center_dis (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _box_hight
     (setq _box_hight (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get motion_planning::camera_msg :md5sum-) "114e47e9e532df1ce24754afff3794bc")
(setf (get motion_planning::camera_msg :datatype-) "motion_planning/camera_msg")
(setf (get motion_planning::camera_msg :definition-)
      "bool find_box_flag
float64 x_center_dis
float64 y_center_dis
float64 box_hight

")



(provide :motion_planning/camera_msg "114e47e9e532df1ce24754afff3794bc")


