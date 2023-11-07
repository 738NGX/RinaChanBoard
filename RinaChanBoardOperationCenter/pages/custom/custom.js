const app=getApp();
const {
    none,horizontalFlip,
    mouth01, mouth02, mouth03, mouth04, mouth05, mouth06, mouth07, mouth08,
    mouth09, mouth10, mouth11, mouth12, mouth13, mouth14, mouth15, mouth16,
    leye01, leye02, leye03, leye04, leye05, leye06, leye07, leye08,
    leye09, leye10, leye11, leye12, leye13, leye14, leye15, leye16,
    reye01, reye02, reye03, reye04, reye05, reye06, reye07, reye08,
    reye09, reye10, reye11, reye12, reye13, reye14, reye15, reye16,
    cheek00,cheek01,cheek02,cheek03,cheek04,
}=require('../../utils/faces.js');

const {
    setMouthByArray,setLeftEyeByArray,setRightEyeByArray,
    setCheekByArray,extractColorsOptimized
}=require('../../utils/face_func.js');

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
                        { src: '/images/leye/01.png', label: '01', bindtap: 'setLeftEye01' },
                        { src: '/images/leye/02.png', label: '02', bindtap: 'setLeftEye02' },
                        { src: '/images/leye/03.png', label: '03', bindtap: 'setLeftEye03' },
                        { src: '/images/leye/04.png', label: '04', bindtap: 'setLeftEye04' },
                        { src: '/images/leye/05.png', label: '05', bindtap: 'setLeftEye05' },
                        { src: '/images/leye/06.png', label: '06', bindtap: 'setLeftEye06' },
                        { src: '/images/leye/07.png', label: '07', bindtap: 'setLeftEye07' },
                        { src: '/images/leye/08.png', label: '08', bindtap: 'setLeftEye08' },
                        { src: '/images/leye/09.png', label: '09', bindtap: 'setLeftEye09' },
                        { src: '/images/leye/10.png', label: '10', bindtap: 'setLeftEye10' },
                        { src: '/images/leye/11.png', label: '11', bindtap: 'setLeftEye11' },
                        { src: '/images/leye/12.png', label: '12', bindtap: 'setLeftEye12' },
                        { src: '/images/leye/13.png', label: '13', bindtap: 'setLeftEye13' },
                        { src: '/images/leye/14.png', label: '14', bindtap: 'setLeftEye14' },
                        { src: '/images/leye/15.png', label: '15', bindtap: 'setLeftEye15' },
                        { src: '/images/leye/16.png', label: '16', bindtap: 'setLeftEye16' },
                    ]
                },
                {
                    name: '右眼',
                    items: [
                        { src: '/images/00.png', label: '00', bindtap: 'resetRightEye' },
                        { src: '/images/reye/01.png', label: '01', bindtap: 'setRightEye01' },
                        { src: '/images/reye/02.png', label: '02', bindtap: 'setRightEye02' },
                        { src: '/images/reye/03.png', label: '03', bindtap: 'setRightEye03' },
                        { src: '/images/reye/04.png', label: '04', bindtap: 'setRightEye04' },
                        { src: '/images/reye/05.png', label: '05', bindtap: 'setRightEye05' },
                        { src: '/images/reye/06.png', label: '06', bindtap: 'setRightEye06' },
                        { src: '/images/reye/07.png', label: '07', bindtap: 'setRightEye07' },
                        { src: '/images/reye/08.png', label: '08', bindtap: 'setRightEye08' },
                        { src: '/images/reye/09.png', label: '09', bindtap: 'setRightEye09' },
                        { src: '/images/reye/10.png', label: '10', bindtap: 'setRightEye10' },
                        { src: '/images/reye/11.png', label: '11', bindtap: 'setRightEye11' },
                        { src: '/images/reye/12.png', label: '12', bindtap: 'setRightEye12' },
                        { src: '/images/reye/13.png', label: '13', bindtap: 'setRightEye13' },
                        { src: '/images/reye/14.png', label: '14', bindtap: 'setRightEye14' },
                        { src: '/images/reye/15.png', label: '15', bindtap: 'setRightEye15' },
                        { src: '/images/reye/16.png', label: '16', bindtap: 'setRightEye16' },
                    ]
                },
                {
                    name: '嘴巴',
                    items: [
                        { src: '/images/00.png', label: '00', bindtap: 'resetMouth' },
                        { src: '/images/mouth/01.png', label: '01', bindtap: 'setMouth01' },
                        { src: '/images/mouth/02.png', label: '02', bindtap: 'setMouth02' },
                        { src: '/images/mouth/03.png', label: '03', bindtap: 'setMouth03' },
                        { src: '/images/mouth/04.png', label: '04', bindtap: 'setMouth04' },
                        { src: '/images/mouth/05.png', label: '05', bindtap: 'setMouth05' },
                        { src: '/images/mouth/06.png', label: '06', bindtap: 'setMouth06' },
                        { src: '/images/mouth/07.png', label: '07', bindtap: 'setMouth07' },
                        { src: '/images/mouth/08.png', label: '08', bindtap: 'setMouth08' },
                        { src: '/images/mouth/09.png', label: '09', bindtap: 'setMouth09' },
                        { src: '/images/mouth/10.png', label: '10', bindtap: 'setMouth10' },
                        { src: '/images/mouth/11.png', label: '11', bindtap: 'setMouth11' },
                        { src: '/images/mouth/12.png', label: '12', bindtap: 'setMouth12' },
                        { src: '/images/mouth/13.png', label: '13', bindtap: 'setMouth13' },
                        { src: '/images/mouth/14.png', label: '14', bindtap: 'setMouth14' },
                        { src: '/images/mouth/15.png', label: '15', bindtap: 'setMouth15' },
                        { src: '/images/mouth/16.png', label: '16', bindtap: 'setMouth16' },
                    ]
                },
                {
                    name: '脸颊',
                    items: [
                        { src: '/images/00.png', label: '00', bindtap: 'resetCheek' },
                        { src: '/images/cheek/01.png', label: '01', bindtap: 'setCheek01' },
                        { src: '/images/cheek/02.png', label: '02', bindtap: 'setCheek02' },
                        { src: '/images/cheek/03.png', label: '03', bindtap: 'setCheek03' },
                        { src: '/images/cheek/04.png', label: '04', bindtap: 'setCheek04' },
                    ]
                },
            ],
    },
    onLoad() {
        this.setData({ cells: app.getGlobalCells() });
        // 添加监听回调
        app.addCellsListener((newCells) => {
            this.setData({ cells: newCells });
        });
    },
    onUnload() {
        // 移除监听器
        app.removeCellsListener(this.cellsListenerCallback);
    },
    toggleColor(e) {
        const index=e.currentTarget.dataset.index;
        const cells=this.data.cells.slice();
        cells[index].color=cells[index].color === 0 ? 1 : 0;
        this.setData({ cells });
        app.setGlobalCells(cells);
        var msgToSend=extractColorsOptimized(cells);
        console.log(msgToSend);
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data:
            {
                uid: 'a8a83e1f0a4c4e42b031e1c323dd9159',
                topic: "RinaChanBoard",
                msg: msgToSend
            },
            header:
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) {
                console.log(res.data);
            }
        })
    },
    resetColors() {
        const cells=this.data.cells.map(cell => ({
            ...cell,
            color: 0
        }));
        this.setData({ cells });
        app.setGlobalCells(cells);
        wx.request({
            url: 'https://api.bemfa.com/api/device/v1/data/1/',
            method: "POST",
            data:
            {
                uid: 'a8a83e1f0a4c4e42b031e1c323dd9159',
                topic: "RinaChanBoard",
                msg: extractColorsOptimized(cells)
            },
            header:
            {
                'content-type': "application/x-www-form-urlencoded"
            },
            success(res) {
                console.log(res.data);
            }
        })
    },
    resetMouth() { setMouthByArray(this,none); },
    setMouth01() { setMouthByArray(this,mouth01); },
    setMouth02() { setMouthByArray(this,mouth02); },
    setMouth03() { setMouthByArray(this,mouth03); },
    setMouth04() { setMouthByArray(this,mouth04); },
    setMouth05() { setMouthByArray(this,mouth05); },
    setMouth06() { setMouthByArray(this,mouth06); },
    setMouth07() { setMouthByArray(this,mouth07); },
    setMouth08() { setMouthByArray(this,mouth08); },
    setMouth09() { setMouthByArray(this,mouth09); },
    setMouth10() { setMouthByArray(this,mouth10); },
    setMouth11() { setMouthByArray(this,mouth11); },
    setMouth12() { setMouthByArray(this,mouth12); },
    setMouth13() { setMouthByArray(this,mouth13); },
    setMouth14() { setMouthByArray(this,mouth14); },
    setMouth15() { setMouthByArray(this,mouth15); },
    setMouth16() { setMouthByArray(this,mouth16); },
    resetLeftEye() { setLeftEyeByArray(this,none); },
    setLeftEye01() { setLeftEyeByArray(this,leye01); },
    setLeftEye02() { setLeftEyeByArray(this,leye02); },
    setLeftEye03() { setLeftEyeByArray(this,leye03); },
    setLeftEye04() { setLeftEyeByArray(this,leye04); },
    setLeftEye05() { setLeftEyeByArray(this,leye05); },
    setLeftEye06() { setLeftEyeByArray(this,leye06); },
    setLeftEye07() { setLeftEyeByArray(this,leye07); },
    setLeftEye08() { setLeftEyeByArray(this,leye08); },
    setLeftEye09() { setLeftEyeByArray(this,leye09); },
    setLeftEye10() { setLeftEyeByArray(this,leye10); },
    setLeftEye11() { setLeftEyeByArray(this,leye11); },
    setLeftEye12() { setLeftEyeByArray(this,leye12); },
    setLeftEye13() { setLeftEyeByArray(this,leye13); },
    setLeftEye14() { setLeftEyeByArray(this,leye14); },
    setLeftEye15() { setLeftEyeByArray(this,leye15); },
    setLeftEye16() { setLeftEyeByArray(this,leye16); },
    resetRightEye() { setRightEyeByArray(this,none); },
    setRightEye01() { setRightEyeByArray(this,reye01); },
    setRightEye02() { setRightEyeByArray(this,reye02); },
    setRightEye03() { setRightEyeByArray(this,reye03); },
    setRightEye04() { setRightEyeByArray(this,reye04); },
    setRightEye05() { setRightEyeByArray(this,reye05); },
    setRightEye06() { setRightEyeByArray(this,reye06); },
    setRightEye07() { setRightEyeByArray(this,reye07); },
    setRightEye08() { setRightEyeByArray(this,reye08); },
    setRightEye09() { setRightEyeByArray(this,reye09); },
    setRightEye10() { setRightEyeByArray(this,reye10); },
    setRightEye11() { setRightEyeByArray(this,reye11); },
    setRightEye12() { setRightEyeByArray(this,reye12); },
    setRightEye13() { setRightEyeByArray(this,reye13); },
    setRightEye14() { setRightEyeByArray(this,reye14); },
    setRightEye15() { setRightEyeByArray(this,reye15); },
    setRightEye16() { setRightEyeByArray(this,reye16); },
    resetCheek() { setCheekByArray(this,cheek00); },
    setCheek01() { setCheekByArray(this,cheek01); },
    setCheek02() { setCheekByArray(this,cheek02); },
    setCheek03() { setCheekByArray(this,cheek03); },
    setCheek04() { setCheekByArray(this,cheek04); },
});