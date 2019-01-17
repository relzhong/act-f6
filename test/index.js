const assert = require('assert');

const act = require('../index');

describe('send card to track read', () => {
  let device;
  it('should open COM3 successfully', () => {
    const { error, data } = act.F6_Connect(3, 9600);
    assert(error === 0);
    device = data.handle;
  });
  it('should read status successfully', () => {
    const res = act.F6_GetSenserDetail(device);
    console.log(res);
    assert(res.error === 0);
  });
  it('should get error successfully', () => {
    let res = act.F6_DenieInsertion(device);
    assert(res.error === 0);
    res = act.F6_Reset(device, 0x30);
    assert(res.error === 0);
  });
  it('should send card position to ic positon', () => {
    let res = act.F6_Entry(device, 0x32);
    assert(res.error === 0);
    res = act.F6_MoveCard(device, 0x31);
    assert(res.error === 0);
  });
  it('should read track successfully', () => {
    const res = act.F6_ReadTracks(device, 0x37, 0x20);
    assert(res.error === 0);
  });

  it('should send card position to receiver', () => {
    const res = act.F6_MoveCard(device, 0x35);
    assert(res.error === 0);
  });

  after(() => {
    act.F6_Disconnect(device);
  });
});

