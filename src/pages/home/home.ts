import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BluetoothSerial } from '@ionic-native/bluetooth-serial';
import { AlertController, ToastController } from 'ionic-angular';
import { AboutPage } from '../about/about'
import { SocialSharing } from '@ionic-native/social-sharing';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {
  pairedList: pairedlist;
  listToggle: boolean = false;
  pairedDeviceID: number = 0;
  dataSend: string = "";
  Binnacle:any[] = [];

  constructor(private socialSharing: SocialSharing, public navCtrl: NavController, private alertCtrl: AlertController, private bluetoothSerial: BluetoothSerial, private toastCtrl: ToastController) {
    this.checkBluetoothEnabled();
    this.Binnacle.push("Esperando comunicación...");
  }

  checkBluetoothEnabled() {
    this.bluetoothSerial.isEnabled().then(success => {
      this.listPairedDevices();
    }, error => {
      this.showError("Habilita el Bluetooth")
    });
  }

  listPairedDevices() {
    this.bluetoothSerial.list().then(success => {
      this.pairedList = success;
      this.listToggle = true;
    }, error => {
      this.showError("Habilita el Bluetooth")
      this.listToggle = false;
    });
  }

  selectDevice() {
    let connectedDevice = this.pairedList[this.pairedDeviceID];
    if (!connectedDevice.address) {
      this.showError('Seleccionar dispositivo para emparejar');
      return;
    }
    let address = connectedDevice.address;
    //let name = connectedDevice.name;

    this.connect(address);
  }

  connect(address) {
    // Attempt to connect device with specified address, call app.deviceConnected if success
    this.bluetoothSerial.connect(address).subscribe(success => {
      this.deviceConnected();
      this.showToast2("Conexión exitosa");
      this.listToggle = false;
    }, error => {
      this.showError("Error al intentar conectar al dispositivo");
    });
  }

  deviceConnected() {
    // Subscribe to data receiving as soon as the delimiter is read
    this.bluetoothSerial.subscribe('\n').subscribe(success => {
      this.handleData(success);
      //this.showToast("Connected Successfullly");
    }, error => {
      this.showError(error);
    });
  }

  deviceDisconnected() {
    // Unsubscribe from data receiving
    this.bluetoothSerial.disconnect();
    this.showToast("Dispositivo desconectado");
  }

  handleData(data) {
    this.showToast(data);
  }

  readData()
  {
    this.bluetoothSerial.read()
    .then((data: any) => {
      this.Binnacle.push(data);
    });
  }

  GotoInfo()
  {
    this.navCtrl.push(AboutPage);
  }

  sendData(letter) {
    this.dataSend+='\n';
    this.showToast(this.dataSend);

    this.bluetoothSerial.write(letter).then(success => {
      this.showToast2('Instrucción enviada');
    }, error => {
      this.showError(error)
    });
  }

  showError(error) {
    let alert = this.alertCtrl.create({
       subTitle: error,
    });
    alert.present();
  }

  showToast(msj) {
    this.Binnacle.push(msj);
  }

  showToast2(msj) {
    const toast = this.toastCtrl.create({
      message: msj,
      duration: 1000
    });
    toast.present();
  }
 
  Share(){
    //this.GenerateData();
    var res = "ARQUITECTURA DE COMPUTADORAS (UMG) - PROYECTO FINAL\n"
    + "ARCHIVO BITÁCORA TORITO (Contiene log de movimientos realizados durante la última sesión)\n\n";

    console.log(this.Binnacle);

    this.Binnacle.forEach(element => {
      res += element+"\n";  
    });
    console.log(res);
    this.socialSharing.share(res, null, null, null);
  }

}

interface pairedlist {
  "class": number,
  "id": string,
  "address": string,
  "name": string
}