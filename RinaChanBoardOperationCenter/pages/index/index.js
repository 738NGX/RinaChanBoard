const app=getApp();
const util=require('../../utils/util');

Page({
    data:  
    {
        device_status: false,       // 默认离线
        power_status: false,        // 默认关闭
        controlling_device: 0,
        next_device: 0,
        devices:['未选择','工程样机','体验机'],
        hidden_modal: true,
        password: '',
    },

    onLoad: function () 
    {
        var that=this
        this.controlling_device=app.get_controlling_device();
        this.next_device=0;
        // 设备断开不会立即显示离线，由于网络情况的复杂性，离线1分钟左右才判断真离线
        // 设置定时器，每五秒请求一下设备状态
        setInterval(function () {
            //console.log("定时请求设备状态,默认五秒");
            wx.request({
                url: 'https://api.bemfa.com/api/device/v1/status/',  //get 设备状态接口，详见巴法云接入文档
                data: 
                {
                    uid: util.device_info[app.get_controlling_device()].uid,
                    topic: util.device_info[app.get_controlling_device()].topic,
                },
                header: 
                {
                    'content-type': "application/x-www-form-urlencoded"
                },
                success(res) 
                {
                    //console.log(res.data)
                    if (res.data.status === "online") 
                    {
                        that.setData({
                            device_status: true
                        })
                    } 
                    else 
                    {
                        that.setData({
                            device_status: false
                        })
                    }
                    //console.log(that.data.device_status)
                }
            })

        }, 5000)
    },

    showModal: function() 
    {
        this.setData({
            hidden_modal: false
        });
    },
    hideModal: function() 
    {
        this.setData({
            hidden_modal: true
        });
    },
    passwordInput: function(e) 
    {
        this.setData({
            password: e.detail.value
        });
    },
    checkPassword: function() 
    {
        var res=(
            (this.data.password=='20040210' && this.next_device==1) ||
            (this.data.password=='5201314' && this.next_device==2)
        );
        if (res)
        {
            this.setData({controlling_device: this.next_device});
            app.set_controlling_device(this.next_device);
            this.hideModal();
        } 
        else 
        {
            wx.showToast({
                title: '密码错误',
                icon: 'none'
            });
        }
    },

    bindPickerChange: function(e) 
    {
        this.next_device=parseInt(e.detail.value);
        if(this.next_device) this.showModal();
        else 
        {
            this.setData({controlling_device: this.next_device});
            app.set_controlling_device(this.next_device);
        }
    },
})  