Component({
    properties: {
      header: {
        type: String,
        value: 'Expand Me'
      }
    },
    data: {
      expanded: false
    },
    methods: {
      toggle() {
        this.setData({
          expanded: !this.data.expanded
        });
      }
    }
  });
  