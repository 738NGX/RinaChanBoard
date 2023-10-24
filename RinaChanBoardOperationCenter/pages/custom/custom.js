const app=getApp();
const {
    none,
    mouth01,mouth02,mouth03,mouth04,mouth05,mouth06,mouth07,mouth08,
    mouth09,mouth10,mouth11,mouth12,mouth13,mouth14,mouth15,mouth16,
    leye01,leye02,leye03,leye04,leye05,leye06,leye07,leye08,
    leye09,leye10,leye11,leye12,leye13,leye14,leye15,leye16,
    reye01,reye02,reye03,reye04,reye05,reye06,reye07,reye08,
    reye09,reye10,reye11,reye12,reye13,reye14,reye15,reye16,
}=require('../../utils/faces.js');

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
    onUnload() {
        // 移除监听器
        app.removeCellsListener(this.cellsListenerCallback);
    },
    toggleColor(e) 
    {
        const index = e.currentTarget.dataset.index;
        const cells = this.data.cells.slice();
        cells[index].color = cells[index].color === 0 ? 1 : 0;
        this.setData({ cells });
        app.setGlobalCells(cells);
    },
    resetColors() 
    {
        const cells = this.data.cells.map(cell => ({
            ...cell,
            color: 0
        }));
        this.setData({ cells });
        app.setGlobalCells(cells);
    },

    // 设置嘴巴
    setMouthByArray(colorArray) 
    {
        const cells = this.data.cells.slice();
        for (let row = 0; row < 8; row++) 
        {
            for (let col = 0; col < 8; col++) 
            {
                const globalRow = row + 8;
                const globalCol = col + 5;
                const index = globalRow * 18 + globalCol;
                cells[index].color = colorArray[row][col];
            }
        }
        this.setData({ cells });
        app.setGlobalCells(cells);
    },
    resetMouth(){this.setMouthByArray(none);},
    setMouth01(){this.setMouthByArray(mouth01);},
    setMouth02(){this.setMouthByArray(mouth02);},
    setMouth03(){this.setMouthByArray(mouth03);},
    setMouth04(){this.setMouthByArray(mouth04);},
    setMouth05(){this.setMouthByArray(mouth05);},
    setMouth06(){this.setMouthByArray(mouth06);},
    setMouth07(){this.setMouthByArray(mouth07);},
    setMouth08(){this.setMouthByArray(mouth08);},
    setMouth09(){this.setMouthByArray(mouth09);},
    setMouth10(){this.setMouthByArray(mouth10);},
    setMouth11(){this.setMouthByArray(mouth11);},
    setMouth12(){this.setMouthByArray(mouth12);},
    setMouth13(){this.setMouthByArray(mouth13);},
    setMouth14(){this.setMouthByArray(mouth14);},
    setMouth15(){this.setMouthByArray(mouth15);},
    setMouth16(){this.setMouthByArray(mouth16);},
    // 设置左眼
    setLeftEyeByArray(colorArray) 
    {
        const cells = this.data.cells.slice();
        for (let row = 0; row < 8; row++) 
        {
            for (let col = 0; col < 8; col++) 
            {
                const globalRow = row + 0;
                const globalCol = col + 0;
                const index = globalRow * 18 + globalCol;
                cells[index].color = colorArray[row][col];
            }
        }
        this.setData({ cells });
        app.setGlobalCells(cells);
    },
    resetLeftEye(){this.setLeftEyeByArray(none);},
    setLeftEye01(){this.setLeftEyeByArray(leye01);},
    setLeftEye02(){this.setLeftEyeByArray(leye02);},
    setLeftEye03(){this.setLeftEyeByArray(leye03);},
    setLeftEye04(){this.setLeftEyeByArray(leye04);},
    setLeftEye05(){this.setLeftEyeByArray(leye05);},
    setLeftEye06(){this.setLeftEyeByArray(leye06);},
    setLeftEye07(){this.setLeftEyeByArray(leye07);},
    setLeftEye08(){this.setLeftEyeByArray(leye08);},
    setLeftEye09(){this.setLeftEyeByArray(leye09);},
    setLeftEye10(){this.setLeftEyeByArray(leye10);},
    setLeftEye11(){this.setLeftEyeByArray(leye11);},
    setLeftEye12(){this.setLeftEyeByArray(leye12);},
    setLeftEye13(){this.setLeftEyeByArray(leye13);},
    setLeftEye14(){this.setLeftEyeByArray(leye14);},
    setLeftEye15(){this.setLeftEyeByArray(leye15);},
    setLeftEye16(){this.setLeftEyeByArray(leye16);},
    // 设置右眼
    setRightEyeByArray(colorArray) 
    {
        const cells = this.data.cells.slice();
        for (let row = 0; row < 8; row++) 
        {
            for (let col = 0; col < 8; col++) 
            {
                const globalRow = row + 0;
                const globalCol = col + 10;
                const index = globalRow * 18 + globalCol;
                cells[index].color = colorArray[row][col];
            }
        }
        this.setData({ cells });
        app.setGlobalCells(cells);
    },
    resetRightEye(){this.setRightEyeByArray(none);},
    setRightEye01(){this.setRightEyeByArray(reye01);},
    setRightEye02(){this.setRightEyeByArray(reye02);},
    setRightEye03(){this.setRightEyeByArray(reye03);},
    setRightEye04(){this.setRightEyeByArray(reye04);},
    setRightEye05(){this.setRightEyeByArray(reye05);},
    setRightEye06(){this.setRightEyeByArray(reye06);},
    setRightEye07(){this.setRightEyeByArray(reye07);},
    setRightEye08(){this.setRightEyeByArray(reye08);},
    setRightEye09(){this.setRightEyeByArray(reye09);},
    setRightEye10(){this.setRightEyeByArray(reye10);},
    setRightEye11(){this.setRightEyeByArray(reye11);},
    setRightEye12(){this.setRightEyeByArray(reye12);},
    setRightEye13(){this.setRightEyeByArray(reye13);},
    setRightEye14(){this.setRightEyeByArray(reye14);},
    setRightEye15(){this.setRightEyeByArray(reye15);},
    setRightEye16(){this.setRightEyeByArray(reye16);},
});