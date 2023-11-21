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

        remoteIP:[192,168,1,120],
        remotePort:1234,
        localPort:4321,

        color:'f971d4',
        color_idx:0,
        color_info:util.color_info,
        color_names:util.color_info.map(color=>color.name),
        colors:util.color_info.map(color=>color.color),

        bright:64,
        bright_idx:0,
        brights:['正常','輝く','眩しい','光害'],
    },

    onLoad: function () 
    {
        var that=this
        this.controlling_device=app.get_controlling_device();
        this.next_device=0;
    },

    inputIP(e)
    {
        let index=parseInt(e.currentTarget.dataset.index);
        let val=parseInt(e.detail.value);
        if(val!=val)
        {
            switch(index)
            {
                case 0: val=192; break;
                case 1: val=168; break;
                case 2: val=  1; break;
                case 3: val=120; break;
            }
        }
        this.data.remoteIP[index]=val;
    },
    inputRemotePort(e)
    {
        let val=parseInt(e.detail.value);
        if(val!=val) val=1234;
        this.data.remotePort=val;
    },
    inputLocalPort(e)
    {
        let val=parseInt(e.detail.value);
        if(val!=val) val=4321;
        this.data.localPort=val;
    },
    updateUDPClient()
    {
        app.updateGlobalUDPClient(this.data.remoteIP,this.data.remotePort,this.data.localPort);
    },
    startUDPClient()
    {
        this.setData({device_status:true})
        app.initUdpSocket();
    },
    stopUDPClient()
    {
        this.setData({device_status:false})
        app.stopUdpSocket();
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
        this.setData({bright:brights[parseInt(e.detail.value)],bright_idx:parseInt(e.detail.value)});
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
            app.globalData.udpSocket.send({
                address: app.globalData.remoteIP,
                port: app.globalData.remotePort,
                message: color_code
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
        app.globalData.udpSocket.send({
            address: app.globalData.remoteIP,
            port: app.globalData.remotePort,
            message: 'requestColor'
        });
        await util.sleep(100);
        let msg=app.globalData.messageList[app.globalData.messageList.length-1].text;
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
            app.globalData.udpSocket.send({
                address: app.globalData.remoteIP,
                port: app.globalData.remotePort,
                message: bright_s
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
        app.globalData.udpSocket.send({
            address: app.globalData.remoteIP,
            port: app.globalData.remotePort,
            message: 'requestBright'
        });
        await util.sleep(100);
        let msg=app.globalData.messageList[app.globalData.messageList.length-1].text;
        this.setData({bright:parseInt(msg),bright_idx:Math.round(parseInt(msg)/64)-1});
    }
})  