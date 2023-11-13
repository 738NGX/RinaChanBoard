const app=getApp();
const util=require('../../utils/util');
const brights=[64,128,192,255];

Page({
    data:  
    {
        device_status: false,       // 默认离线
        
        devices:['未选择','工程样机','体验机'],
        controlling_device: 0,
        next_device: 0,
        
        hidden_modal: true,
        password: '',

        color:'',
        color_idx:0,
        color_info:util.color_info,
        color_names:util.color_info.map(color=>color.name),
        colors:util.color_info.map(color=>color.color),

        bright:64,
        bright_idx:0,
        brights:brights,
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
        },5000)
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

    bindControllingDeviceChange: function(e) 
    {
        this.next_device=parseInt(e.detail.value);
        if(this.next_device) this.showModal();
        else 
        {
            this.setData({controlling_device: this.next_device});
            app.set_controlling_device(this.next_device);
        }
    },

    bindColorChange: function(e)
    {
        this.setData({color:util.color_info[parseInt(e.detail.value)].color});
    },

    bindColorInput: function (e) 
    {
        this.setData({color: e.detail.value})
    },
    
    bindBrightChange: function(e)
    {
        this.setData({bright:brights[parseInt(e.detail.value)]});
    },

    bindBrightInput: function (e) 
    {
        this.setData({bright: parseInt(e.detail.value)})
    },

    uploadColors()  
    {
        let color_code='#'+this.data.color.toLowerCase();
        var RegExp = /^#[0-9a-f]{6}$/i;
        if(RegExp.test(color_code))
        {
            wx.request({
                url: 'https://api.bemfa.com/api/device/v1/data/1/',
                method: "POST",
                data:
                {
                    uid: util.device_info[app.get_controlling_device()].uid,
                    topic: util.device_info[app.get_controlling_device()].topic,
                    msg: color_code,
                },
                header:
                {
                    'content-type': "application/x-www-form-urlencoded"
                },
                success(res) 
                {
                    console.log(res.data);
                }
            });
        }
        else
        {
            wx.showToast({
                title: '不是有效的颜色',
                icon: 'none'
            });
        }
    },
    async downloadColors()
    {
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data:
            {
                uid: util.device_info[app.get_controlling_device()].uid,
                topic: util.device_info[app.get_controlling_device()].topic,
                msg: "requestColor"
            },
            header:
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) 
            {
                console.log(res.data);
            }
        });

        await util.sleep(800);
        let msg='';
        
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/', //get接口，详见巴法云接入文档
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
                msg=res.data.msg;
                console.log(msg)
            }
        })
        await util.sleep(1000);
        console.log(msg)
        this.setData({color:msg.slice(1)});
    },

    uploadBright()  
    {
        let bright_s=this.data.bright.toString();
        if(bright_s.length==1) bright_s='B'+'00'+bright_s;
        else if(bright_s.length==2) bright_s='B'+'0'+bright_s;
        else if(bright_s.length==3) bright_s='B'+bright_s;
        console.log(bright_s)
        var RegExp = /^B[0-9]{3}$/i;
        if(RegExp.test(bright_s)&&this.data.bright>=0&&this.data.bright<=255)
        {
            wx.request({
                url: 'https://api.bemfa.com/api/device/v1/data/1/',
                method: "POST",
                data:
                {
                    uid: util.device_info[app.get_controlling_device()].uid,
                    topic: util.device_info[app.get_controlling_device()].topic,
                    msg: bright_s,
                },
                header:
                {
                    'content-type': "application/x-www-form-urlencoded"
                },
                success(res) 
                {
                    console.log(res.data);
                }
            });
        }
        else
        {
            wx.showToast({
                title: '不是有效的亮度',
                icon: 'none'
            });
        }
    },
    async downloadBright()
    {
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data:
            {
                uid: util.device_info[app.get_controlling_device()].uid,
                topic: util.device_info[app.get_controlling_device()].topic,
                msg: "requestBright"
            },
            header:
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) 
            {
                console.log(res.data);
            }
        });

        await util.sleep(800);
        let msg='';
        
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/', //get接口，详见巴法云接入文档
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
                msg=res.data.msg;
                console.log(msg)
            }
        })
        await util.sleep(1000);
        console.log(msg)
        this.setData({bright:parseInt(msg)});
    }
})  