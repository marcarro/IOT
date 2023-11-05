const mysql = require('../database/db')

class MainController {

    async logValue(req, res){
        console.log(req.params.value)
        console.log(req.params.deviceID)
        if(req.params.deviceID != null && req.params.value != null) {
            let deviceID = req.params.deviceID
            let value = req.params.value;
            var sql = `INSERT INTO DatosSensor (idSensor, valor, fecha) VALUES (${deviceID}, ${value}, NOW())`;
            mysql.query(sql, (error,data,fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        status: 200,
                        message: "Log uploaded successfully",
                        affectedRows: data.affectedRows
                    })
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }

    async logState(req, res){
      console.log(req.params.state)
      console.log(req.params.deviceId)
      if(req.params.deviceID != null && req.params.state != null) {
        let deviceID = req.params.deviceID
        let state = req.params.state;
        var sql = `INSERT INTO DatosActuador (idActuador, estado, fecha) VALUES (${deviceID}, '${state}', NOW())`;
        mysql.query(sql, (error,data,fields) => {
          if(error) {
            res.status(500)
            res.send(error.message)
          } else {
            console.log(data)
            res.json({
              status: 200,
              message: "Log uploaded successfully",
              affectedRows: data.affectedRows
            })
          }
        })
      } else {
        res.send('Por favor llena todos los datos!')
      }
    }
    
    async getLogs(req,res){
        console.log("Get Logs")
        console.log(req.params.deviceID)
        if(req.params.deviceID!=null){
            let deviceID = req.params.deviceID;
            var sql = `SELECT * FROM DatosSensor where idSensor='${deviceID}'`
            mysql.query(sql, (error, data, fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        data
                    })
                }
            })
        }
    }
}

const tempController = new MainController()
module.exports = tempController;