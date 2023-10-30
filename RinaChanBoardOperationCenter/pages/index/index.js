const app = getApp()

Page({
    data: {
        uid: 'a8a83e1f0a4c4e42b031e1c323dd9159',
        topic: "RinaChanBoard",
        device_status: false,       // 默认离线
        power_status: false         // 默认关闭
    },
    
    switchLED: function (e) {
        var that = this;
        var newStatus = e.detail.value; // 获取switch的状态
    
        // 根据开关状态更新数据
        that.setData({
            powerstatus: newStatus
        });
    
        var msgToSend = newStatus ? "on" : "off";   // 根据状态发送消息
    
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data: 
            {
                uid: that.data.uid,
                topic: that.data.topic,
                msg: msgToSend
            },
            header: 
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) 
            {
                console.log(res.data);
            }
        })
    },
    onLoad: function () {
        var that = this

        // 请求设备状态
        // 设备断开不会立即显示离线，由于网络情况的复杂性，离线1分钟左右才判断真离线
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/status/', // 状态api接口，详见巴法云接入文档
            data: {
                uid: that.data.uid,
                topic: that.data.topic,
            },
            header: {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) {
                console.log(res.data)
                if (res.data.status === "online") {
                    that.setData({
                        device_status: true
                    })
                } else {
                    that.setData({
                        device_status: false
                    })
                }
                console.log(that.data.device_status)
            }
        })

        // 请求询问设备开关/状态
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/', // get接口，详见巴法云接入文档
            data: {
                uid: that.data.uid,
                topic: that.data.topic,
            },
            header: {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) {
                console.log(res.data)
                if (res.data.msg === "on") {
                    that.setData({
                        powerstatus: true
                    })
                }
                console.log(that.data.powerstatus)
            }
        })


        // 设置定时器，每五秒请求一下设备状态
        setInterval(function () {
            //console.log("定时请求设备状态,默认五秒");
            wx.request({
                url: 'https://api.bemfa.com/api/device/v1/status/',  //get 设备状态接口，详见巴法云接入文档
                data: {
                    uid: that.data.uid,
                    topic: that.data.topic,
                },
                header: {
                    'content-type': "application/x-www-form-urlencoded"
                },
                success(res) {
                    console.log(res.data)
                    if (res.data.status === "online") {
                        that.setData({
                            device_status: true
                        })
                    } else {
                        that.setData({
                            device_status: false
                        })
                    }
                    //console.log(that.data.device_status)
                }
            })

            // 请求询问设备开关/状态
            wx.request({
                url: 'https://api.bemfa.com/api/device/v1/data/1/', // get接口，详见巴法云接入文档
                data: {
                    uid: that.data.uid,
                    topic: that.data.topic,
                },
                header: {
                    'content-type': "application/x-www-form-urlencoded"
                },
                success(res) {
                    console.log(res.data)
                    if (res.data.msg === "on") {
                        that.setData({
                            powerstatus: true
                        })
                    }
                    console.log(that.data.powerstatus)
                }
            })

        }, 5000)
    }
})  