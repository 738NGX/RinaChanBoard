const app=getApp();
const 
{
    none,mouth,leye,reye,cheek00,cheek,
}=require('../../utils/faces.js');

const 
{
    setMouthByArray,setLeftEyeByArray,setRightEyeByArray,
    setCheekByArray,update_face_to_server,setColorsByString
}=require('../../utils/face_func.js');

const util=require('../../utils/util.js')

Page({
    data:
    {
        cells: [],
        features:
        [
            {
                name: '左眼',
                items: [
                    { src: '/images/00.png', label: '00', bindtap: 'resetLeftEye' },
                    { src: '/images/leye/01.png', label: '01', bindtap: 'setLeftEye' ,'data-index': 1},
                    { src: '/images/leye/02.png', label: '02', bindtap: 'setLeftEye' ,'data-index': 2},
                    { src: '/images/leye/03.png', label: '03', bindtap: 'setLeftEye' ,'data-index': 3},
                    { src: '/images/leye/04.png', label: '04', bindtap: 'setLeftEye' ,'data-index': 4},
                    { src: '/images/leye/05.png', label: '05', bindtap: 'setLeftEye' ,'data-index': 5},
                    { src: '/images/leye/06.png', label: '06', bindtap: 'setLeftEye' ,'data-index': 6},
                    { src: '/images/leye/07.png', label: '07', bindtap: 'setLeftEye' ,'data-index': 7},
                    { src: '/images/leye/08.png', label: '08', bindtap: 'setLeftEye' ,'data-index': 8},
                    { src: '/images/leye/09.png', label: '09', bindtap: 'setLeftEye' ,'data-index': 9},
                    { src: '/images/leye/10.png', label: '10', bindtap: 'setLeftEye' ,'data-index': 10},
                    { src: '/images/leye/11.png', label: '11', bindtap: 'setLeftEye' ,'data-index': 11},
                    { src: '/images/leye/12.png', label: '12', bindtap: 'setLeftEye' ,'data-index': 12},
                    { src: '/images/leye/13.png', label: '13', bindtap: 'setLeftEye' ,'data-index': 13},
                    { src: '/images/leye/14.png', label: '14', bindtap: 'setLeftEye' ,'data-index': 14},
                    { src: '/images/leye/15.png', label: '15', bindtap: 'setLeftEye' ,'data-index': 15},
                    { src: '/images/leye/16.png', label: '16', bindtap: 'setLeftEye' ,'data-index': 16},
                ]
            },
            {
                name: '右眼',
                items: [
                    { src: '/images/00.png', label: '00', bindtap: 'resetRightEye' },
                    { src: '/images/reye/01.png', label: '01', bindtap: 'setRightEye' ,'data-index': 1},
                    { src: '/images/reye/02.png', label: '02', bindtap: 'setRightEye' ,'data-index': 2},
                    { src: '/images/reye/03.png', label: '03', bindtap: 'setRightEye' ,'data-index': 3},
                    { src: '/images/reye/04.png', label: '04', bindtap: 'setRightEye' ,'data-index': 4},
                    { src: '/images/reye/05.png', label: '05', bindtap: 'setRightEye' ,'data-index': 5},
                    { src: '/images/reye/06.png', label: '06', bindtap: 'setRightEye' ,'data-index': 6},
                    { src: '/images/reye/07.png', label: '07', bindtap: 'setRightEye' ,'data-index': 7},
                    { src: '/images/reye/08.png', label: '08', bindtap: 'setRightEye' ,'data-index': 8},
                    { src: '/images/reye/09.png', label: '09', bindtap: 'setRightEye' ,'data-index': 9},
                    { src: '/images/reye/10.png', label: '10', bindtap: 'setRightEye' ,'data-index': 10},
                    { src: '/images/reye/11.png', label: '11', bindtap: 'setRightEye' ,'data-index': 11},
                    { src: '/images/reye/12.png', label: '12', bindtap: 'setRightEye' ,'data-index': 12},
                    { src: '/images/reye/13.png', label: '13', bindtap: 'setRightEye' ,'data-index': 13},
                    { src: '/images/reye/14.png', label: '14', bindtap: 'setRightEye' ,'data-index': 14},
                    { src: '/images/reye/15.png', label: '15', bindtap: 'setRightEye' ,'data-index': 15},
                    { src: '/images/reye/16.png', label: '16', bindtap: 'setRightEye' ,'data-index': 16},
                ]
            },
            {
                name: '嘴巴',
                items: [
                    { src: '/images/00.png', label: '00', bindtap: 'resetMouth' },
                    { src: '/images/mouth/01.png', label: '01', bindtap: 'setMouth' ,'data-index': 1},
                    { src: '/images/mouth/02.png', label: '02', bindtap: 'setMouth' ,'data-index': 2},
                    { src: '/images/mouth/03.png', label: '03', bindtap: 'setMouth' ,'data-index': 3},
                    { src: '/images/mouth/04.png', label: '04', bindtap: 'setMouth' ,'data-index': 4},
                    { src: '/images/mouth/05.png', label: '05', bindtap: 'setMouth' ,'data-index': 5},
                    { src: '/images/mouth/06.png', label: '06', bindtap: 'setMouth' ,'data-index': 6},
                    { src: '/images/mouth/07.png', label: '07', bindtap: 'setMouth' ,'data-index': 7},
                    { src: '/images/mouth/08.png', label: '08', bindtap: 'setMouth' ,'data-index': 8},
                    { src: '/images/mouth/09.png', label: '09', bindtap: 'setMouth' ,'data-index': 9},
                    { src: '/images/mouth/10.png', label: '10', bindtap: 'setMouth' ,'data-index': 10},
                    { src: '/images/mouth/11.png', label: '11', bindtap: 'setMouth' ,'data-index': 11},
                    { src: '/images/mouth/12.png', label: '12', bindtap: 'setMouth' ,'data-index': 12},
                    { src: '/images/mouth/13.png', label: '13', bindtap: 'setMouth' ,'data-index': 13},
                    { src: '/images/mouth/14.png', label: '14', bindtap: 'setMouth' ,'data-index': 14},
                    { src: '/images/mouth/15.png', label: '15', bindtap: 'setMouth' ,'data-index': 15},
                    { src: '/images/mouth/16.png', label: '16', bindtap: 'setMouth' ,'data-index': 16},
                ]
            },
            {
                name: '脸颊',
                items: [
                    { src: '/images/00.png', label: '00', bindtap: 'resetCheek' },
                    { src: '/images/cheek/01.png', label: '01', bindtap: 'setCheek' ,'data-index': 1},
                    { src: '/images/cheek/02.png', label: '02', bindtap: 'setCheek' ,'data-index': 2},
                    { src: '/images/cheek/03.png', label: '03', bindtap: 'setCheek' ,'data-index': 3},
                    { src: '/images/cheek/04.png', label: '04', bindtap: 'setCheek' ,'data-index': 4},
                ]
            },
        ],
    },
    onLoad() 
    {
        this.setData({ cells: app.getGlobalCells() });
        // 添加监听回调
        app.addCellsListener((newCells) => {
            this.setData({ cells: newCells });
        });
    },
    onUnload() 
    {
        // 移除监听器
        app.removeCellsListener(this.cellsListenerCallback);
    },
    toggleColor(e) 
    {
        const index=e.currentTarget.dataset.index;
        const cells=this.data.cells.slice();
        cells[index].color=cells[index].color === 0 ? 1 : 0;
        this.setData({ cells });
        app.setGlobalCells(cells);
    },
    uploadColors() 
    {
        const cells=this.data.cells.slice();
        update_face_to_server(cells);
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
                msg: "requestFace"
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
        
        await util.sleep(500);
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
        await util.sleep(600);
        console.log('start.')
        setColorsByString(this,msg);
    },
    resetColors() 
    {
        const cells=this.data.cells.map(cell => ({
            ...cell,
            color: 0
        }));
        this.setData({ cells });
        app.setGlobalCells(cells);
    },
    resetMouth() { setMouthByArray(this,none); },
    setMouth(e) { setMouthByArray(this,mouth[e.currentTarget.dataset.index-1]); },
    resetLeftEye() { setLeftEyeByArray(this,none); },
    setLeftEye(e) { setLeftEyeByArray(this,leye[e.currentTarget.dataset.index-1]); },
    resetRightEye() { setRightEyeByArray(this,none); },
    setRightEye(e) { setRightEyeByArray(this,reye[e.currentTarget.dataset.index-1]); },
    resetCheek() { setCheekByArray(this,cheek00); },
    setCheek(e) { setCheekByArray(this,cheek[e.currentTarget.dataset.index-1]); },
});