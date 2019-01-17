const ffi = require('ffi');
const ref = require('ref');
const path = require('path');
const Struct = require('ref-struct');
const ArrayType = require('ref-array');
const R = require('ramda');
const Decimal = require('decimal.js');
const hardware = {};
const stack = require('callsite');

function hazardous(location) {
  const electronRegex = /[\\/]electron\.asar[\\/]/;
  const asarRegex = /^(?:^\\\\\?\\)?(.*\.asar)[\\/](.*)/;
  /* convert path when use electron asar unpack
   */
  if (!path.isAbsolute(location)) {
    return location;
  }

  if (electronRegex.test(location)) {
    return location;
  }

  const matches = asarRegex.exec(location);
  if (!matches || matches.length !== 3) {
    return location;
  }

  /* Skip monkey patching when an electron method is in the callstack. */
  const skip = stack().some(site => {
    const siteFile = site.getFileName();
    return /^ELECTRON_ASAR/.test(siteFile) || electronRegex.test(siteFile);
  });

  return skip ? location : location.replace(/\.asar([\\/])/, '.asar.unpacked$1');
}

/**
   * 字符串转Hex Buffer
   * @param {String} req 字符 { 0 ~ F }
   * @param {Number} length 长度, 自动补长
   * @param {Number} type 拼接方式 { 0: 右边补0, 1: 左边补0 }
   * @return {Buffer} res
   */
function str2Hex(req, length, type) {
  if (length) {
    if (type) {
      // 左边补0
      if (req.length % 2) {
        req = '0' + req;
      }
      const surplusNum = length * 2 - req.length;
      const surplus = R.reduce(R.concat, '', R.repeat('0', surplusNum));
      req = R.splitEvery(2, surplus + req);

    } else {
      // 默认右边补0
      if (req.length % 2) {
        req = req + '0';
      }
      const surplusNum = length * 2 - req.length;
      const surplus = R.reduce(R.concat, '', R.repeat('0', surplusNum));
      req = R.splitEvery(2, req + surplus);
    }
  } else {
    if (req.length % 2) {
      req = req + '0';
    }
    req = R.splitEvery(2, req);
  }

  let buf = Buffer.from('');
  req.forEach(i => { buf = Buffer.concat([ buf, Buffer.alloc(1, new Decimal('0x' + i).toNumber()) ]); });
  return buf;
}

/**
 * Hex Buffer转字符串
 * @param {Buffer} req 字符
 * @return {String} res
 */
function hex2Str(req) {
  let dec = '';
  for (let i = 0; i < req.length; i++) {
    let d = new Decimal(req.readUIntBE(i, 1)).toHex().slice(2, 4)
      .toUpperCase();
    d = d.length % 2 ? '0' + d : '' + d;
    dec = dec + d;
  }
  return dec;
}

const TRACKINFO = Struct({
  Contents: ArrayType(ArrayType('char', 256), 3),
  Lengths: ArrayType('int', 3),
  Status: ArrayType('byte', 3),
});

const libact = ffi.Library(hazardous(path.join(__dirname, './lib/F6_30_API')), {
  F6_Connect: [ 'int', [ 'int', 'int', 'pointer' ]],
  F6_Disconnect: [ 'int', [ 'int' ]],
  F6_Cancel: [ 'int', [ 'int' ]],
  F6_Reset: [ 'int', [ 'int', 'byte', 'pointer', 'pointer' ]],
  F6_Entry: [ 'int', [ 'int', 'byte' ]],
  F6_PermitInsertion: [ 'int', [ 'int' ]],
  F6_DenieInsertion: [ 'int', [ 'int' ]],
  F6_MoveCard: [ 'int', [ 'int', 'byte' ]],
  F6_LedControl: [ 'int', [ 'int', 'byte' ]],
  F6_SetBaudRate: [ 'int', [ 'int', 'byte' ]],
  F6_GetStatus: [ 'int', [ 'int', 'pointer' ]],
  F6_GetSenserDetail: [ 'int', [ 'int', 'pointer' ]], // 接收状态信息的数组，长度为 9 个字节 byte。
  F6_GetSenserLevel: [ 'int', [ 'int', 'pointer' ]], // 接收状态信息的数组，长度为 9 个字节 float。
  F6_DetectICCType: [ 'int', [ 'int', 'pointer' ]],
  F6_DetectRFCardType: [ 'int', [ 'int', 'pointer' ]],
  F6_IccPowerOn: [ 'int', [ 'int' ]],
  F6_IccPowerOff: [ 'int', [ 'int' ]],
  F6_CpuActivate: [ 'int', [ 'int', 'byte', 'pointer', 'pointer', 'pointer' ]],
  F6_CpuDeactivate: [ 'int', [ 'int' ]],
  F6_CpuTransmit: [ 'int', [ 'int', 'byte', 'string', 'int', 'pointer', 'pointer' ]],
  F6_ReadTracks: [ 'int', [ 'int', 'byte', 'int', ref.refType(TRACKINFO) ]],
});

hardware.F6_Connect = (port, baudRate) => {
  try {
    if (!baudRate) baudRate = 9600;
    const handle = ref.alloc(ref.types.long);
    const res = libact.F6_Connect(port, baudRate, handle);
    if (res !== 0) {
      return { error: res };
    }
    return { error: 0, data: { handle: handle.deref() } };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_Disconnect = handle => {
  try {
    const res = libact.F6_Disconnect(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * RESET_NOACTION	0x30
 * RESET_EJECT	0x31
 * RESET_RETRACT	0x32
 * RESET_RETAIN	0x33
 */
hardware.F6_Reset = (handle, action) => {
  try {
    if (!action) action = 0x30;
    const len = ref.alloc(ref.types.byte);
    const data = ref.alloc(ref.types.char);
    const res = libact.F6_Reset(handle, action, data, len);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_Cancel = handle => {
  try {
    const res = libact.F6_Cancel(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * ENTRY_FROMFRONT 0x30
 * ENTRY_FROMREAR 0x32
 */
hardware.F6_Entry = (handle, action) => {
  try {
    if (!action) action = 0x30;
    const res = libact.F6_Entry(handle, action);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_PermitInsertion = handle => {
  try {
    const res = libact.F6_PermitInsertion(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_DenieInsertion = handle => {
  try {
    const res = libact.F6_DenieInsertion(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * MOVE_TORFPOS 0x30
 * MOVE_TOICPOS 0x31
 * MOVE_TOFRONT 0x32
 * MOVE_TOREAR 0x33
 * MOVE_TOGATEPOS 0x34
 * MOVE_RETRACT 0x35
 * MOVE_TOREREADPOS 0x36
 */
hardware.F6_MoveCard = (handle, action) => {
  try {
    if (!action) action = 0x30;
    const res = libact.F6_MoveCard(handle, action);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * LED_OFF 0x30
 * LED_LIGHTEN 0x31
 * LED_BLINK 0x32
 */
hardware.F6_LedControl = (handle, action) => {
  try {
    if (!action) action = 0x30;
    const res = libact.F6_LedControl(handle, action);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_SetBaudRate = (handle, baudRate) => {
  try {
    if (!baudRate) baudRate = 9600;
    const res = libact.F6_SetBaudRate(handle, baudRate);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * CRS_CARDINGATEPOS 0x30
 * CRS_CARDINFRONT 0x31
 * CRS_CARDINRFPOS		0x32
 * CRS_CARDINICPOS		0x33
 * CRS_CARDINREAR		0x34
 * CRS_NOCARDIN		0x35
 * CRS_NOTINSTDPOS		0x36
 * CRS_CARDINREREADPOS	0x37
 */
hardware.F6_GetStatus = handle => {
  try {
    const status = ref.alloc(ref.types.byte);
    const res = libact.F6_GetStatus(handle, status);
    if (res === 0) {
      return { error: 0, data: { status: status.deref() } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * PSS1 =0x31, 表示有卡；=0x30，表示无卡
 * PSS2 =0x31, 表示有卡；=0x30，表示无卡
 * PSS3 =0x31, 表示有卡；=0x30，表示无卡
 * PSS4 =0x31, 表示有卡；=0x30，表示无卡
 * PSS5 =0x31, 表示有卡；=0x30，表示无卡
 * PSS6 闸门传感器，未使用
 * PSS7 卡箱传感器，=0x31, 表示卡箱到位；=0x30，表示卡箱未到位
 * PSS8 回收箱传感器，=0x31, 表示回收箱到位；=0x30，表示回收箱未到位
 * PSS9 卡预空传感器，=0x31, 表示卡足；=0x30，表示卡少
 */
hardware.F6_GetSenserDetail = handle => {
  try {
    const status = new Buffer(10 * ref.types.byte.size);
    const res = libact.F6_GetSenserDetail(handle, status);
    if (res === 0) {
      return { error: 0, data: { details: ref.reinterpret(status, 9) } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_GetSenserLevel = handle => {
  try {
    const status = new Buffer(10 * ref.types.float.size);
    const res = libact.F6_GetSenserLevel(handle, status);
    if (res === 0) {
      return { error: 0, data: { details: ref.reinterpret(status, 9) } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * ICCTYPE_AT24C01		0x30
 * ICCTYPE_AT24C02		0x31
 * ICCTYPE_AT24C04		0x32
 * ICCTYPE_AT24C08		0x33
 * ICCTYPE_AT24C16		0x34
 * ICCTYPE_AT24C32		0x35
 * ICCTYPE_AT24C64		0x36
 * ICCTYPE_AT45DB041	0x37
 * ICCTYPE_AT88SC102	0x38
 * ICCTYPE_AT88SC1604	0x39
 * ICCTYPE_AT88SC1608	0x3A
 * ICCTYPE_SLE4442		0x3B
 * ICCTYPE_SLE4428		0x3C
 * ICCTYPE_T0_CPU		0x3D
 * ICCTYPE_T1_CPU		0x3E
 * ICCTYPE_UNKNOWN		0xFF
 */
hardware.F6_DetectICCType = handle => {
  try {
    const status = ref.alloc(ref.types.byte);
    const res = libact.F6_DetectICCType(handle, status);
    if (res === 0) {
      return { error: 0, data: { status: status.deref() } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};


/*
 * RFCTYPE_MIFARE_UL	0x30
 * RFCTYPE_MIFARE_S50	0x31
 * RFCTYPE_MIFARE_S70	0x32
 * RFCTYPE_TYPEA_CPU	0x33
 * RFCTYPE_TYPEB_CPU	0x42
 * RFCTYPE_UNKNOWN		0xFF
 */
hardware.F6_DetectRFCardType = handle => {
  try {
    const status = ref.alloc(ref.types.byte);
    const res = libact.F6_DetectRFCardType(handle, status);
    if (res === 0) {
      return { error: 0, data: { status: status.deref() } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_IccPowerOn = handle => {
  try {
    const res = libact.F6_IccPowerOn(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_IccPowerOff = handle => {
  try {
    const res = libact.F6_IccPowerOff(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_CpuDeactivate = handle => {
  try {
    const res = libact.F6_CpuDeactivate(handle);
    if (res === 0) {
      return { error: 0 };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * VOLTAGE_1_8	0x30
 * VOLTAGE_3	0x31
 * VOLTAGE_5	0x32
 */
hardware.F6_CpuActivate = (handle, voltage) => {
  try {
    if (!voltage) voltage = 0x32;
    const len = ref.alloc(ref.types.int);
    const data = ref.alloc(ref.types.byte);
    const protocol = ref.alloc(ref.types.byte);
    const res = libact.F6_CpuActivate(handle, voltage, protocol, data, len);
    const outData = ref.reinterpret(data, len.deref());
    if (res === 0) {
      return { error: 0, data: { protocol: protocol.deref(), ATRBuff: hex2Str(outData) } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

hardware.F6_CpuTransmit = (handle, protocol, sendBuff) => {
  try {
    sendBuff = str2Hex(sendBuff);
    const len = ref.alloc(ref.types.byte);
    const data = ref.alloc(ref.types.char);
    const res = libact.F6_CpuTransmit(handle, protocol, sendBuff, sendBuff.length, data, len);
    const outData = ref.reinterpret(data, len.deref());
    if (res === 0) {
      return { error: 0, data: { recvBuff: hex2Str(outData) } };
    }
    return { error: res };
  } catch (e) {
    return { error: -1 };
  }
};

/*
 * TRACK_1	0x10
 * TRACK_2	0x20
 * TRACK_3	0x40
 * READ_ASCII	0x37
 * READ_BINARY	0x38
 */
hardware.F6_ReadTracks = (handle, mode, trackId) => {
  try {
    const trackInfo = new TRACKINFO();
    const res = libact.F6_ReadTracks(handle, mode, trackId, trackInfo.ref());
    if (res === 0) {
      const track1Length = trackInfo.Lengths[0];
      const track2Length = trackInfo.Lengths[1];
      const track3Length = trackInfo.Lengths[2];
      return { error: 0, data: { track1: track1Length ? Buffer.from(trackInfo.Contents[0]).slice(0, track1Length).toString() : '',
        track2: track2Length ? Buffer.from(trackInfo.Contents[1]).slice(0, track2Length).toString() : '',
        track3: track3Length ? Buffer.from(trackInfo.Contents[2]).slice(0, track3Length).toString() : '' } };
    }
    return { error: res };
  } catch (e) {
    console.log(e);
    return { error: -1 };
  }
};

module.exports = hardware;
