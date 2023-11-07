const app=getApp();

function extractColorsOptimized(cells) 
{
    let binaryString='';

    cells.forEach(cell => {
        binaryString+=cell.color;
    });

    let hexString='';
    for (let i=0;i<binaryString.length;i+=4) 
    {
        const fourBinaryDigits=binaryString.slice(i, i+4);
        const hexDigit=parseInt(fourBinaryDigits, 2).toString(16);
        hexString+=hexDigit;
    }

    return hexString;
}

// 设置嘴巴
function setMouthByArray(obj,colorArray) {
    const cells=obj.data.cells.slice();
    for (let row=0; row<8; row++) {
        for (let col=0; col<8; col++) {
            const globalRow=row+8;
            const globalCol=col+5;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    obj.setData({ cells });
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
}

// 设置左眼
function setLeftEyeByArray(obj,colorArray) {
    const cells=obj.data.cells.slice();
    for (let row=0; row<8; row++) {
        for (let col=0; col<8; col++) {
            const globalRow=row+0;
            const globalCol=col+0;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    obj.setData({ cells });
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
}

// 设置右眼
function setRightEyeByArray(obj,colorArray) {
    const cells=obj.data.cells.slice();
    for (let row=0; row<8; row++) {
        for (let col=0; col<8; col++) {
            const globalRow=row+0;
            const globalCol=col+10;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    obj.setData({ cells });
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
}

// 设置脸颊
function setCheekByArray(obj,colorArray) {
    var horizontalArray=horizontalFlip(colorArray.slice());
    const cells=obj.data.cells.slice();
    for (let row=0; row<2; row++) {
        for (let col=0; col<5; col++) {
            const globalRow=row+8;
            const globalCol=col+0;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    for (let row=0; row<2; row++) {
        for (let col=0; col<5; col++) {
            const globalRow=row+8;
            const globalCol=col+13;
            const index=globalRow*18+globalCol;
            cells[index].color=horizontalArray[row][col];
        }
    }
    obj.setData({ cells });
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
}

module.exports = {
    setMouthByArray,setLeftEyeByArray,setRightEyeByArray,
    setCheekByArray,extractColorsOptimized
};