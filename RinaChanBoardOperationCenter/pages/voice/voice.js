const app=getApp();

Page({
    data: 
    {
        cells: [],
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
})