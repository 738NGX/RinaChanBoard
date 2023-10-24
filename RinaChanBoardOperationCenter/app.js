// app.js
App({
    globalData: {
        userInfo: null,
        cells: [],
        cellsListeners: []
    },
    onLaunch() {
        this.globalData.cells = Array.from({ length: 16 * 18 }, (_, index) => ({
            row: Math.floor(index / 18),
            col: index % 18,
            color: 0
        }))
    },
    setGlobalCells(newCells) 
    {
        this.globalData._cells = newCells;
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
        this.globalData.cellsListeners = this.globalData.cellsListeners.filter(listener => listener !== callback);
    },
})
