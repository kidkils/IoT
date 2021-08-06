Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';


var ctxNode1 = document.getElementById("chartNode1");
var dataNode1 = new Chart(ctxNode1, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: "Sessions",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: [],
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: 100,
          maxTicksLimit: 7
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var sourceNode1 = new EventSource("/data/{{ measurement }}/1")
sourceNode1.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (dataNode1.data.labels.length === 7) {
        dataNode1.data.labels.shift();
        dataNode1.data.datasets[0].data.shift();
    }

    dataNode1.data.labels.push(data.time);
    dataNode1.data.datasets[0].data.push(data.value);
    dataNode1.update();
}

// chart node 2
var ctxNode2 = document.getElementById("chartNode2");
var dataNode2 = new Chart(ctxNode2, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: "Sessions",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: [],
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: 100,
          maxTicksLimit: 7
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var sourceNode2 = new EventSource("/data/{{ measurement }}/2")
sourceNode2.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (dataNode2.data.labels.length === 7) {
        dataNode2.data.labels.shift();
        dataNode2.data.datasets[0].data.shift();
    }

    dataNode2.data.labels.push(data.time);
    dataNode2.data.datasets[0].data.push(data.value);
    dataNode2.update();
}

// chart node 3
var ctxNode3 = document.getElementById("chartNode3");
var dataNode3 = new Chart(ctxNode3, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: "Sessions",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: [],
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: 100,
          maxTicksLimit: 7
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var sourceNode3 = new EventSource("/data/{{ measurement }}/3")
sourceNode3.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (dataNode3.data.labels.length === 7) {
        dataNode3.data.labels.shift();
        dataNode3.data.datasets[0].data.shift();
    }

    dataNode3.data.labels.push(data.time);
    dataNode3.data.datasets[0].data.push(data.value);
    dataNode3.update();
}

// chart node 4
var ctxNode4 = document.getElementById("chartNode4");
var dataNode4 = new Chart(ctxNode4, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: "Sessions",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: [],
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: 100,
          maxTicksLimit: 7
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var sourceNode4 = new EventSource("/data/{{ measurement }}/4")
sourceNode4.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (dataNode4.data.labels.length === 7) {
        dataNode4.data.labels.shift();
        dataNode4.data.datasets[0].data.shift();
    }

    dataNode4.data.labels.push(data.time);
    dataNode4.data.datasets[0].data.push(data.value);
    dataNode4.update();
}

// chart node 5
var ctxNode5 = document.getElementById("chartNode5");
var dataNode5 = new Chart(ctxNode5, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: "Sessions",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: [],
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: 100,
          maxTicksLimit: 7
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var sourceNode5 = new EventSource("/data/{{ measurement }}/5")
sourceNode5.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (dataNode5.data.labels.length === 7) {
        dataNode5.data.labels.shift();
        dataNode5.data.datasets[0].data.shift();
    }

    dataNode5.data.labels.push(data.time);
    dataNode5.data.datasets[0].data.push(data.value);
    dataNode5.update();
}

// chart node 6
var ctxNode6 = document.getElementById("chartNode6");
var dataNode6 = new Chart(ctxNode6, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: "Sessions",
      lineTension: 0.3,
      backgroundColor: "rgba(2,117,216,0.2)",
      borderColor: "rgba(2,117,216,1)",
      pointRadius: 5,
      pointBackgroundColor: "rgba(2,117,216,1)",
      pointBorderColor: "rgba(255,255,255,0.8)",
      pointHoverRadius: 5,
      pointHoverBackgroundColor: "rgba(2,117,216,1)",
      pointHitRadius: 50,
      pointBorderWidth: 2,
      data: [],
    }],
  },
  options: {
    scales: {
      xAxes: [{
        time: {
          unit: 'date'
        },
        gridLines: {
          display: false
        },
        ticks: {
          maxTicksLimit: 7
        }
      }],
      yAxes: [{
        ticks: {
          min: 0,
          max: 100,
          maxTicksLimit: 7
        },
        gridLines: {
          color: "rgba(0, 0, 0, .125)",
        }
      }],
    },
    legend: {
      display: false
    }
  }
});

var sourceNode6 = new EventSource("/data/{{ measurement }}/6")
sourceNode6.onmessage = function (event) {
    const data = JSON.parse(event.data);
    if (dataNode6.data.labels.length === 7) {
        dataNode6.data.labels.shift();
        dataNode6.data.datasets[0].data.shift();
    }

    dataNode6.data.labels.push(data.time);
    dataNode6.data.datasets[0].data.push(data.value);
    dataNode6.update();
}
