enum control {
  BEAT = 0,             // msg length = 0B
  ZER_IMU,              // msg length = 0B
  FLY_TKO,              // msg length = 4B
  FLY_ALT,              // msg length = 4B
  FLY_LND,              // msg length = 0B
  SET_PHI,              // msg length = 4B
  SET_THT,              // msg length = 4B
  SET_PSI,              // msg length = 4B
  GET_ATT,              // msg length = 0B
  SET_OMG,              // msg length = 4xint32 = 16B
  SET_XYZ,              // msg length = 3xfp32 = 12B
  GET_XYZ,              // msg length = 0B
  SET_STP,              // msg legnth = 0B (EMERGENCY STOP!)
  GET_BAT,              // msg length = 0B (reply with 1xfp32 = 4B)
  GET_STT,              // msg length = 0B (reply with state struct aka drone)
  REBOOT,               // msg length = 0B
  SET_KP,               // msg length = 4B
  SET_KI,               // msg length = 4B
  SET_KD,               // msg length = 4B
  CMD_SIZE
};

const uint8_t cmd_size[] = {0,0,4,4,0,4,4,4,0,16,12,0,0,0,0,0,4,4,4};

struct CommandData {
    uint8_t cmd;
    union {
        uint8_t raw[16];  // Maximum data length based on the largest expected data size (16 bytes for SET_OMG)
        float values[4];  // For commands that need float values (like SET_OMG which needs 4 floats = 4x4B = 16B)
        uint32_t intArr[4]; //
        uint32_t intValue;  // For commands that need a single integer value
    };
};
