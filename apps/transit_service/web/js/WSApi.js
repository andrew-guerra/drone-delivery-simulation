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

            // get the battery percentage value
            const batteryPercentage1 = dataJson["Drone-1"]["battery-percentage"];
            const batteryPercentage2 = dataJson["Drone-2"]["battery-percentage"];
            const batteryPercentage3 = dataJson["Drone-3"]["battery-percentage"];
            const batteryPercentage4 = dataJson["Drone-4"]["battery-percentage"];
            // set the hue value based on the battery percentage
            const hue1 = (batteryPercentage1/100) * 120; //120 is value for green
            const hue2 = (batteryPercentage2/100) * 120; //120 is value for green
            const hue3 = (batteryPercentage3/100) * 120; //120 is value for green
            const hue4 = (batteryPercentage4/100) * 120; //120 is value for green
            // Set the saturation and lightness values to a fixed value (you can adjust these values as desired)
            const saturation = 100;
            const lightness = 50;
            // Set the color of the text using the HSL color mode
            $('#drone1-battery').css("color", "hsl(" + hue1 + ", " + saturation + "%, " + lightness + "%)");
            $('#drone2-battery').css("color", "hsl(" + hue2 + ", " + saturation + "%, " + lightness + "%)");
            $('#drone3-battery').css("color", "hsl(" + hue3 + ", " + saturation + "%, " + lightness + "%)");
            $('#drone4-battery').css("color", "hsl(" + hue4 + ", " + saturation + "%, " + lightness + "%)");
            
            $('#time-elapsed').text(`${dataJson["time-elapsed"].toFixed(2)} Seconds`)
            
            $('#drone1-battery').text(`Battery: ${dataJson["Drone-1"]["battery-percentage"].toFixed(2)}%`)
            $('#drone1-distance').text(`Distance Traveled: ${dataJson["Drone-1"]["distance"]}`)
            $('#drone1-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-1"]["deliveries"]}`)

            $('#drone2-battery').text(`Battery: ${dataJson["Drone-2"]["battery-percentage"].toFixed(2)}%`)
            $('#drone2-distance').text(`Distance Traveled: ${dataJson["Drone-2"]["distance"]}`)
            $('#drone2-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-2"]["deliveries"]}`)

            $('#drone3-battery').text(`Battery: ${dataJson["Drone-3"]["battery-percentage"].toFixed(2)}%`)
            $('#drone3-distance').text(`Distance Traveled: ${dataJson["Drone-3"]["distance"]}`)
            $('#drone3-passengers-delivered').text(`Passengers Delivered: ${dataJson["Drone-3"]["deliveries"]}`)

            $('#drone4-battery').text(`Battery: ${dataJson["Drone-4"]["battery-percentage"].toFixed(2)}%`)
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