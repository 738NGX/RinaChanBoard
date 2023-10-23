const app=getApp();
const {
    none,
    mouth01,mouth02,mouth03,mouth04,mouth05,mouth06,mouth07,mouth08,
    mouth09,mouth10,mouth11,mouth12,mouth13,mouth14,mouth15,mouth16,
    leye01,leye02,leye03,leye04,leye05,leye06,leye07,leye08,
    leye09,leye10,leye11,leye12,leye13,leye14,leye15,leye16,
}=require('../../utils/faces.js');

Page({
    data: 
    {
        cells: app.globalData.cells
    },
    toggleColor(e) 
    {
        const index = e.currentTarget.dataset.index;
        const cells = this.data.cells.slice();
        cells[index].color = cells[index].color === 0 ? 1 : 0;
        this.setData({ cells });
        app.globalData.cells = cells;
    },
    resetColors() 
    {
        const cells = this.data.cells.map(cell => ({
            ...cell,
            color: 0
        }));
        this.setData({ cells });
        app.globalData.cells = cells;
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
        getApp().globalData.cells = cells;
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
                const globalRow = row + 8;
                const globalCol = col + 5;
                const index = globalRow * 18 + globalCol;
                cells[index].color = colorArray[row][col];
            }
        }
        this.setData({ cells });
        getApp().globalData.cells = cells;
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
});