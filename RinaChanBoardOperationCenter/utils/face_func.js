const app=getApp();
const util=require('./util.js')

const {
    none,mouth,leye,reye,cheek00,cheek,
}=require('./faces.js');

function horizontalFlip(array) 
{
    return array.map(row => row.slice().reverse());
}

function update_face_to_server(cells)
{
    var UDPSocket=app.globalData.udpSocket;
    UDPSocket.send({
        address: app.globalData.remoteIP,
        port: app.globalData.remotePort,
        message: extractColorsOptimized(cells)
    });
    console.log('sended')
}

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

function setColorsByString(obj,hexString) 
{
    let binaryString = '';
    for (let i = 0; i < hexString.length; i++) {
        let hexDigit = hexString[i];
        let binaryDigits = parseInt(hexDigit, 16).toString(2).padStart(4, '0');
        binaryString += binaryDigits;
    }

    var cells=Array.from({ length: 16*18 }, (_, index) => (
    {
        row: Math.floor(index / 18),
        col: index % 18,
        color: 0
    }));

    for (let i = 0; i < cells.length; i++) {
        cells[i].color = binaryString[i] === '1' ? 1 : 0;
    }
    
    obj.setData({ cells });
    app.setGlobalCells(cells);
}

function set_face(obj,leye_idx,reye_idx,mouth_idx,cheek_idx)
{
    var set_leye=leye_idx ? leye[leye_idx-1] : none;
    var set_reye=reye_idx ? reye[reye_idx-1] : none;
    var set_mouth=mouth_idx ? mouth[mouth_idx-1] : none;
    var set_cheek=cheek_idx ? cheek[cheek_idx-1] : cheek00;

    const cells=obj.data.cells.map(cell => ({
        ...cell,
        color: 0
    }));
    for (let row=0; row<8; row++) 
    {
        for (let col=0; col<8; col++) 
        {
            const globalRow=row+0;
            const globalCol=col+0;
            const index=globalRow*18+globalCol;
            cells[index].color=set_leye[row][col];
        }
    }
    for (let row=0; row<8; row++) 
    {
        for (let col=0; col<8; col++) 
        {
            const globalRow=row+0;
            const globalCol=col+10;
            const index=globalRow*18+globalCol;
            cells[index].color=set_reye[row][col];
        }
    }
    for (let row=0; row<8; row++) 
    {
        for (let col=0; col<8; col++) 
        {
            const globalRow=row+8;
            const globalCol=col+5;
            const index=globalRow*18+globalCol;
            cells[index].color=set_mouth[row][col];
        }
    }
    for (let row=0; row<2; row++) 
    {
        for (let col=0; col<5; col++) 
        {
            const globalRow=row+8;
            const globalCol=col+0;
            const index=globalRow*18+globalCol;
            cells[index].color=set_cheek[row][col];
        }
    }
    var horizontalArray=horizontalFlip(set_cheek.slice());
    for (let row=0; row<2; row++) 
    {
        for (let col=0; col<5; col++) 
        {
            const globalRow=row+8;
            const globalCol=col+13;
            const index=globalRow*18+globalCol;
            cells[index].color=horizontalArray[row][col];
        }
    }
    obj.setData({ cells });
    app.setGlobalCells(cells);
}

function resetColors(obj) 
{
    const cells=obj.data.cells.map(cell => ({
        ...cell,
        color: 0
    }));
    obj.setData({ cells });
    app.setGlobalCells(cells);
    update_face_to_server(cells);
}

// 设置嘴巴
function setMouthByArray(obj,colorArray) 
{
    const cells=obj.data.cells.slice();
    for (let row=0; row<8; row++) 
    {
        for (let col=0; col<8; col++) 
        {
            const globalRow=row+8;
            const globalCol=col+5;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    obj.setData({ cells });
    app.setGlobalCells(cells);
}

// 设置左眼
function setLeftEyeByArray(obj,colorArray) 
{
    const cells=obj.data.cells.slice();
    for (let row=0; row<8; row++) 
    {
        for (let col=0; col<8; col++) 
        {
            const globalRow=row+0;
            const globalCol=col+0;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    obj.setData({ cells });
    app.setGlobalCells(cells);
}

// 设置右眼
function setRightEyeByArray(obj,colorArray) 
{
    const cells=obj.data.cells.slice();
    for (let row=0; row<8; row++) 
    {
        for (let col=0; col<8; col++) 
        {
            const globalRow=row+0;
            const globalCol=col+10;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    obj.setData({ cells });
    app.setGlobalCells(cells);
}

// 设置脸颊
function setCheekByArray(obj,colorArray) 
{
    var horizontalArray=horizontalFlip(colorArray.slice());
    const cells=obj.data.cells.slice();
    for (let row=0; row<2; row++) 
    {
        for (let col=0; col<5; col++) 
        {
            const globalRow=row+8;
            const globalCol=col+0;
            const index=globalRow*18+globalCol;
            cells[index].color=colorArray[row][col];
        }
    }
    for (let row=0; row<2; row++) 
    {
        for (let col=0; col<5; col++) 
        {
            const globalRow=row+8;
            const globalCol=col+13;
            const index=globalRow*18+globalCol;
            cells[index].color=horizontalArray[row][col];
        }
    }
    obj.setData({ cells });
    app.setGlobalCells(cells);
}

module.exports = {
    setMouthByArray,setLeftEyeByArray,setRightEyeByArray,
    setCheekByArray,extractColorsOptimized,resetColors,
    set_face,update_face_to_server,setColorsByString
};