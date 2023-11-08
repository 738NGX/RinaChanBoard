// app.js
App({
    globalData: 
    {
        controlling_device: 0,
        cells: [],              // 全局表情数组
        cellsListeners: [],     // 表情数组变化监听
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

    get_controlling_device()
    {
        return this.globalData.controlling_device;
    },

    set_controlling_device(n)
    {
        this.globalData.controlling_device=n;
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
