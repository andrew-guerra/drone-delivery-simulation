function WSApi(host = null) {
    var self = this;
    var hostname = host != null ? host : location.hostname+(location.port ? ':'+location.port: '');
    this.socket = new WebSocket("ws://" + hostname, "web_server");
    this.callbacks = {};
    this.requestId = 0;
    this.id = null;

    this.onmessage = null;

    this.socket.onmessage = function (msg) {
        var data = JSON.parse(msg.data);
        const dataJson = data.details

        if(data.event == "data_update") {
            console.log("event = ",data);
            // update fields for data in html
            
            $('#drone1-battery').text(`Battery: ${dataJson["Drone-1"]["battery-percentage"]}%`)
            $('#drone1-distance').text(`Distance Traveled: ${dataJson["Drone-1"]["distance"]}`)
            $('#drone1-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-1"]["deliveries"]}`)

            $('#drone2-battery').text(`Battery: ${dataJson["Drone-2"]["battery-percentage"]}%`)
            $('#drone2-distance').text(`Distance Traveled: ${dataJson["Drone-2"]["distance"]}`)
            $('#drone2-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-2"]["deliveries"]}`)

            $('#drone3-battery').text(`Battery: ${dataJson["Drone-3"]["battery-percentage"]}%`)
            $('#drone3-distance').text(`Distance Traveled: ${dataJson["Drone-3"]["distance"]}`)
            $('#drone3-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-3"]["deliveries"]}`)

            $('#drone4-battery').text(`Battery: ${dataJson["Drone-4"]["battery-percentage"]}%`)
            $('#drone4-distance').text(`Distance Traveled: ${dataJson["Drone-4"]["distance"]}`)
            $('#drone4-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-4"]["deliveries"]}`)
        }

        if (typeof(data) == 'number') {
            self.id = +msg.data;
            self.connected = true;
            //console.log(self.id);
            return;
        }

        if ("id" in data && data.id in self.callbacks) {
            //console.log(data);
            self.callbacks[data.id](data);
        }

        if (self.onmessage) {
            self.onmessage(msg, data);
        }
    }

    this.connect = new Promise(function(resolve, reject) {
        self.socket.onopen = function() {
            resolve(self.socket);
        };
        self.socket.onerror = function(err) {
            console.log(err);
            reject(err);
        }
        self.socket.onclose = function (event) {
            console.log(event);
        }
    });

    this.connected = false;

    this.connect.then(function() {
        //self.connected = true;
    });
}

WSApi.prototype.sendPostCommand = function(cmd, data, calcVal) {
    console.log(this.id);
    return this.sendCommand(cmd, data, calcVal, true);
}

WSApi.prototype.sendCommand = function(cmd, data, calcVal, isPost = false) {
    let self = this;

    if (self.connected) {
        data.command = cmd;
        data.id = this.requestId;

        if (isPost) {
        $.ajax({
            type: "POST",
            url: "/post/"+self.id,
            //data: JSON.stringify({command: "mouseClicked", output: output}),
            data: JSON.stringify(data),
            success: function(res) { console.log(res); },
            //error: function(res) { console.log(res); },
            dataType: "json"
            });
        }
        else {
            this.socket.send(JSON.stringify(data));
        }

        let promise = new Promise(function(resolve, reject) {
            self.callbacks[self.requestId] = function(data) {
                if (calcVal) {
                    resolve(calcVal(data)); 
                }
                else {
                    resolve(data);
                }
                delete self.callbacks[data.id];
            }
        });
        this.requestId++;
        return promise;
    }
    else {
        return new Promise(function(resolve, reject) {
            self.connect.then(function() {
                self.connected = true;
                    self.sendCommand(cmd, data, calcVal).then(
                        function(data) {
                            resolve(data);
                        });
                }
            );
        });
    }

}