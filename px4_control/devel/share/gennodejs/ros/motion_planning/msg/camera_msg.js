// Auto-generated. Do not edit!

// (in-package motion_planning.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class camera_msg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.find_box_flag = null;
      this.x_center_dis = null;
      this.y_center_dis = null;
      this.box_hight = null;
    }
    else {
      if (initObj.hasOwnProperty('find_box_flag')) {
        this.find_box_flag = initObj.find_box_flag
      }
      else {
        this.find_box_flag = false;
      }
      if (initObj.hasOwnProperty('x_center_dis')) {
        this.x_center_dis = initObj.x_center_dis
      }
      else {
        this.x_center_dis = 0.0;
      }
      if (initObj.hasOwnProperty('y_center_dis')) {
        this.y_center_dis = initObj.y_center_dis
      }
      else {
        this.y_center_dis = 0.0;
      }
      if (initObj.hasOwnProperty('box_hight')) {
        this.box_hight = initObj.box_hight
      }
      else {
        this.box_hight = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type camera_msg
    // Serialize message field [find_box_flag]
    bufferOffset = _serializer.bool(obj.find_box_flag, buffer, bufferOffset);
    // Serialize message field [x_center_dis]
    bufferOffset = _serializer.float64(obj.x_center_dis, buffer, bufferOffset);
    // Serialize message field [y_center_dis]
    bufferOffset = _serializer.float64(obj.y_center_dis, buffer, bufferOffset);
    // Serialize message field [box_hight]
    bufferOffset = _serializer.float64(obj.box_hight, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type camera_msg
    let len;
    let data = new camera_msg(null);
    // Deserialize message field [find_box_flag]
    data.find_box_flag = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [x_center_dis]
    data.x_center_dis = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [y_center_dis]
    data.y_center_dis = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [box_hight]
    data.box_hight = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 25;
  }

  static datatype() {
    // Returns string type for a message object
    return 'motion_planning/camera_msg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '114e47e9e532df1ce24754afff3794bc';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool find_box_flag
    float64 x_center_dis
    float64 y_center_dis
    float64 box_hight
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new camera_msg(null);
    if (msg.find_box_flag !== undefined) {
      resolved.find_box_flag = msg.find_box_flag;
    }
    else {
      resolved.find_box_flag = false
    }

    if (msg.x_center_dis !== undefined) {
      resolved.x_center_dis = msg.x_center_dis;
    }
    else {
      resolved.x_center_dis = 0.0
    }

    if (msg.y_center_dis !== undefined) {
      resolved.y_center_dis = msg.y_center_dis;
    }
    else {
      resolved.y_center_dis = 0.0
    }

    if (msg.box_hight !== undefined) {
      resolved.box_hight = msg.box_hight;
    }
    else {
      resolved.box_hight = 0.0
    }

    return resolved;
    }
};

module.exports = camera_msg;
