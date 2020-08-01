; Auto-generated. Do not edit!


(cl:in-package motion_planning-msg)


;//! \htmlinclude camera_msg.msg.html

(cl:defclass <camera_msg> (roslisp-msg-protocol:ros-message)
  ((find_box_flag
    :reader find_box_flag
    :initarg :find_box_flag
    :type cl:boolean
    :initform cl:nil)
   (x_center_dis
    :reader x_center_dis
    :initarg :x_center_dis
    :type cl:float
    :initform 0.0)
   (y_center_dis
    :reader y_center_dis
    :initarg :y_center_dis
    :type cl:float
    :initform 0.0)
   (box_hight
    :reader box_hight
    :initarg :box_hight
    :type cl:float
    :initform 0.0))
)

(cl:defclass camera_msg (<camera_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <camera_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'camera_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name motion_planning-msg:<camera_msg> is deprecated: use motion_planning-msg:camera_msg instead.")))

(cl:ensure-generic-function 'find_box_flag-val :lambda-list '(m))
(cl:defmethod find_box_flag-val ((m <camera_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motion_planning-msg:find_box_flag-val is deprecated.  Use motion_planning-msg:find_box_flag instead.")
  (find_box_flag m))

(cl:ensure-generic-function 'x_center_dis-val :lambda-list '(m))
(cl:defmethod x_center_dis-val ((m <camera_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motion_planning-msg:x_center_dis-val is deprecated.  Use motion_planning-msg:x_center_dis instead.")
  (x_center_dis m))

(cl:ensure-generic-function 'y_center_dis-val :lambda-list '(m))
(cl:defmethod y_center_dis-val ((m <camera_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motion_planning-msg:y_center_dis-val is deprecated.  Use motion_planning-msg:y_center_dis instead.")
  (y_center_dis m))

(cl:ensure-generic-function 'box_hight-val :lambda-list '(m))
(cl:defmethod box_hight-val ((m <camera_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motion_planning-msg:box_hight-val is deprecated.  Use motion_planning-msg:box_hight instead.")
  (box_hight m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <camera_msg>) ostream)
  "Serializes a message object of type '<camera_msg>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'find_box_flag) 1 0)) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'x_center_dis))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'y_center_dis))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'box_hight))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <camera_msg>) istream)
  "Deserializes a message object of type '<camera_msg>"
    (cl:setf (cl:slot-value msg 'find_box_flag) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'x_center_dis) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'y_center_dis) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'box_hight) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<camera_msg>)))
  "Returns string type for a message object of type '<camera_msg>"
  "motion_planning/camera_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'camera_msg)))
  "Returns string type for a message object of type 'camera_msg"
  "motion_planning/camera_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<camera_msg>)))
  "Returns md5sum for a message object of type '<camera_msg>"
  "114e47e9e532df1ce24754afff3794bc")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'camera_msg)))
  "Returns md5sum for a message object of type 'camera_msg"
  "114e47e9e532df1ce24754afff3794bc")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<camera_msg>)))
  "Returns full string definition for message of type '<camera_msg>"
  (cl:format cl:nil "bool find_box_flag~%float64 x_center_dis~%float64 y_center_dis~%float64 box_hight~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'camera_msg)))
  "Returns full string definition for message of type 'camera_msg"
  (cl:format cl:nil "bool find_box_flag~%float64 x_center_dis~%float64 y_center_dis~%float64 box_hight~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <camera_msg>))
  (cl:+ 0
     1
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <camera_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'camera_msg
    (cl:cons ':find_box_flag (find_box_flag msg))
    (cl:cons ':x_center_dis (x_center_dis msg))
    (cl:cons ':y_center_dis (y_center_dis msg))
    (cl:cons ':box_hight (box_hight msg))
))
