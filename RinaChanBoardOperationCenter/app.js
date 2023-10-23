// app.js
App({
  onLaunch() {
    // 展示本地存储能力
    const logs = wx.getStorageSync('logs') || []
    logs.unshift(Date.now())
    wx.setStorageSync('logs', logs)

    // 登录
    wx.login({
      success: res => {
        // 发送 res.code 到后台换取 openId, sessionKey, unionId
      }
    })

    this.globalData.cells = Array.from({ length: 16 * 18 }, (_, index) => ({
      row: Math.floor(index / 18),
      col: index % 18,
      color: 0
    }))
  },
  globalData: {
    userInfo: null,
    cells: []
  }
})
