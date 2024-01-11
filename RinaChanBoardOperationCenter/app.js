// app.js
App({
    globalData: 
    {
        controlling_device: 0,
        cells: [],              // 全局表情数组
        cellsListeners: [],     // 表情数组变化监听
        
        udpSocket:{},
        udp_client_on:false,
        
        remoteIP:'192.168.111.250',
        remotePort:1234,
        
        localPort:4321,
        
        messageList:[],
    },
    
    onLaunch() 
    {
        // 全局变量初始化
        this.globalData.cells=Array.from({ length: 16*18 }, (_, index) => (
        {
            row: Math.floor(index / 18),
            col: index % 18,
            color: 0
        }));
        this.globalData.controlling_device=0;
        
    },

    initUdpSocket() 
    {
        this.globalData.udpSocket=wx.createUDPSocket();
        this.globalData.udp_client_on=true;
        if(this.globalData.udpSocket === null) 
        {
            console.log('暂不支持');
            this.globalData.udp_client_on=false;
            return;
        }

        this.globalData.udpSocket.bind(this.globalData.localPort);
    
        this.globalData.udpSocket.onListening(function(res){
            console.log('监听中Res ==='+res)
        })
    
        let that=this
        this.globalData.udpSocket.onMessage(function (res) {
            console.log('remoteInfo.size ==='+res.remoteInfo.size)
            if (res.remoteInfo.size > 0) 
            {
                //console.log('UDP接收数据 '+res.remoteInfo.size+' 字节：'+JSON.stringify(res,null,'\t'));
        
                let unit8Arr=new Uint8Array(res.message);
                let encodedString=String.fromCharCode.apply(null, unit8Arr);
                let escStr=escape(encodedString);
                let decodedString=decodeURIComponent(escStr);
                
                console.log('str==='+decodedString);
                let list=that.globalData.messageList;
                let obj=
                {
                    text: decodedString,
                    from: 1
                }
                list.push(obj);
                that.globalData.messageList=list;
            }
        })
    },

    stopUdpSocket()
    {
        this.globalData.udp_client_on=false;
        this.globalData.udpSocket.close();
    },

    get_controlling_device()
    {
        return this.globalData.controlling_device;
    },

    set_controlling_device(n)
    {
        this.globalData.controlling_device=n;
    },

    updateGlobalUDPClient(ip,remotePort,localPort)
    {
        this.globalData.remoteIP=ip[0].toString()+'.'+ip[1].toString()+'.'+ip[2].toString()+'.'+ip[3].toString();
        this.globalData.remotePort=remotePort;
        this.globalData.localPort=localPort;
        console.log([this.globalData.remoteIP,this.globalData.remotePort,this.globalData.localPort])
    },
    
    /**
     * 全局表情数组相关函数
     */
    setGlobalCells(newCells) 
    {
        this.globalData.cells=newCells;
        // 通知所有监听者
        this.globalData.cellsListeners.forEach(callback => callback(newCells));
    },
    getGlobalCells() 
    {
        return this.globalData.cells;
    },
    addCellsListener(callback) 
    {
        this.globalData.cellsListeners.push(callback);
    },
    removeCellsListener(callback) 
    {
        this.globalData.cellsListeners=this.globalData.cellsListeners.filter(listener => listener !== callback);
    },
})
