using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Robotic_arm_interface
{
    public partial class Form1 : Form
    {
        RoboticArmModel roboticArmModel;
        Dictionary<char, KeyType> keyTypeValuePairs;
        Dictionary<char, MotorName> keyMotorNamePairs;
        Dictionary<char, Button> keyButtonPairs;
        Form controlHelp;
        Form conectionHelp;
        Color colorPressing;

        char baseA = 'A';
        char baseH = 'D';
        char hombroA = 'W';
        char hombroH = 'S';
        char codoA = 'd'; //4 en teclado numerico
        char codoH = 'a'; //1 en teclado numerico
        char muñecaA = 'f'; //4 en teclado numerico
        char muñecaH = 'c'; //1 en teclado numerico
        char garraC = ' ';

        public Form1()
        {
            InitializeComponent();
            setup();
            
        }
        public void serialPortWrite(char cmd)
        {
            int c = (int)cmd;
            string test = c.ToString(); 
            if (serialPort1.IsOpen)
            {
                lblTest.Text = "se envio el comando " + cmd;
                lblTest2.Text = "se envio el valor " + test;
                //lblTest.Text = "";
                serialPort1.Write(""+cmd);
            }
            else
            {
                this.conectionHelp.ShowDialog();
            }
            

            //lblTest.Text = "se envio el comando " + cmd;

        }

        public void setColorPressing(Color color)
        {
            this.colorPressing = color;
        }

        private void btnConectPort_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen == false)
            {
                try
                {
                    serialPort1.Open();
                }
                catch (Exception ex)
                {
                    this.conectionHelp.ShowDialog();
                }
            }
           
        }

        private void setup()
        {
            this.roboticArmModel = new RoboticArmModel(this);
            this.keyTypeValuePairs = new Dictionary<char, KeyType>();
            this.keyMotorNamePairs = new Dictionary<char, MotorName>();
            this.keyButtonPairs = new Dictionary<char, Button>();
            this.controlHelp = new FormHelp();
            this.conectionHelp = new FormConectMsg();
            this.colorPressing = Color.LightGray;

            setKeyTypeValues();
            setMotorNameValues();
            setKeyButtonValues();
        }

        private void setKeyTypeValues()
        {
            this.keyTypeValuePairs.Add(baseA, KeyType.ANTIHORARIA);
            this.keyTypeValuePairs.Add(hombroA, KeyType.ANTIHORARIA);
            this.keyTypeValuePairs.Add(codoA, KeyType.ANTIHORARIA);
            this.keyTypeValuePairs.Add(muñecaA, KeyType.ANTIHORARIA);

            this.keyTypeValuePairs.Add(baseH, KeyType.HORARIA);
            this.keyTypeValuePairs.Add(hombroH, KeyType.HORARIA);
            this.keyTypeValuePairs.Add(codoH, KeyType.HORARIA);
            this.keyTypeValuePairs.Add(muñecaH, KeyType.HORARIA);

        }

        private void setMotorNameValues()
        {
            this.keyMotorNamePairs.Add(baseA, MotorName.BASE);
            this.keyMotorNamePairs.Add(baseH, MotorName.BASE);

            this.keyMotorNamePairs.Add(hombroA, MotorName.HOMBRO);
            this.keyMotorNamePairs.Add(hombroH, MotorName.HOMBRO);

            this.keyMotorNamePairs.Add(codoA, MotorName.CODO);
            this.keyMotorNamePairs.Add(codoH, MotorName.CODO);

            this.keyMotorNamePairs.Add(muñecaA, MotorName.MUNIECA);
            this.keyMotorNamePairs.Add(muñecaH, MotorName.MUNIECA);
        }

        private void setKeyButtonValues()
        {
            this.keyButtonPairs.Add(baseA, btnBaseA);
            this.keyButtonPairs.Add(baseH, btnBaseH);

            this.keyButtonPairs.Add(hombroA, btnHombroA);
            this.keyButtonPairs.Add(hombroH, btnHombroH);

            this.keyButtonPairs.Add(codoA, btnCodoA);
            this.keyButtonPairs.Add(codoH, btnCodoH);

            this.keyButtonPairs.Add(muñecaA, btnMuñecaA);
            this.keyButtonPairs.Add(muñecaH, btnMuñecaH);

            this.keyButtonPairs.Add(garraC, btnGarraA_S);
        }

        private KeyType getKeyType(char key)
        {
            KeyType type;
            this.keyTypeValuePairs.TryGetValue(key, out type); //no se si funciona jaja
            return type;
        }
        private MotorName getKeyMotor(char key)
        {
            MotorName mName;
            this.keyMotorNamePairs.TryGetValue(key, out mName); 
            return mName;
        }

        private Button getButton(char key)
        {
            Button button;
            this.keyButtonPairs.TryGetValue(key, out button);
            return button;
        }


        private void keyDown(char key)
        {
            Button button;

            if (this.keyTypeValuePairs.ContainsKey(key))
            {
                this.roboticArmModel.keyDown(getKeyMotor(key), getKeyType(key));
            }
            else if(key == garraC)
            {
               // this.roboticArmModel.garraCmd();
            }

            if (this.keyButtonPairs.ContainsKey(key))
            {
                button = getButton(key);
                this.pressButton(button);
            }
        }

        private void keyUp(char key)
        {
            Button button;
            if (this.keyTypeValuePairs.ContainsKey(key))
            {
                this.roboticArmModel.keyUp(getKeyMotor(key), getKeyType(key));
            }
            else if (key == garraC)
            {
                this.roboticArmModel.garraCmd();
            }

            if (this.keyButtonPairs.ContainsKey(key))
            {
                button = getButton(key);
                button.BackColor = Color.LightGray;              
            }
        }



        private void pressButton(Button button)
        {
            button.BackColor = this.colorPressing;
        }


        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            this.keyDown((char)e.KeyValue);
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            this.keyUp((char)e.KeyValue);
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            this.KeyPreview = true; //IMPORTANTISIMO PARA LA ENTRADA DE COMANDOS POR TECLADO
            rbtnManual.Checked = true;
            rbtnGrabar.Checked = false;
            rbtnEjecutar.Checked = false;

        }

        private void btnHelp_Click(object sender, EventArgs e)
        {
            this.controlHelp.ShowDialog();
        }

        private void rbtnManual_CheckedChanged(object sender, EventArgs e)
        {
            if (rbtnManual.Checked == true)
                this.roboticArmModel.manual();
        }

        private void rbtnGrabar_CheckedChanged(object sender, EventArgs e)
        {
            if (rbtnGrabar.Checked == true)
                this.roboticArmModel.grabar();
        }

        private void rbtnEjecutar_CheckedChanged(object sender, EventArgs e)
        {
            if (rbtnEjecutar.Checked == true)
                this.roboticArmModel.ejecutar();
        }

        private void rbtnManPote_CheckedChanged(object sender, EventArgs e)
        {
            if (rbtnManPote.Checked == true)
                this.roboticArmModel.manualPote();
        }
        private void rbtnConfiguracion_CheckedChanged(object sender, EventArgs e)
        {
            if (rbtnConfiguracion.Checked == true)
                this.roboticArmModel.configurar();
        }

        private void btnBaseA_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(baseA);
        }

        private void btnBaseA_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(baseA);
        }

        private void btnBaseH_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(baseH);
        }

        private void btnBaseH_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(baseH);
        }

        private void btnHombroA_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(hombroA);
        }

        private void btnHombroA_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(hombroA);
        }

        private void btnHombroH_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(hombroH);
        }

        private void btnHombroH_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(hombroH);
        }

        private void btnCodoA_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(codoA);
        }

        private void btnCodoA_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(codoA);
        }

        private void btnCodoH_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(codoH);
        }

        private void btnCodoH_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(codoH);
        }

        private void btnMuñecaA_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(muñecaA);
        }

        private void btnMuñecaA_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(muñecaA);
        }

        private void btnMuñecaH_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(muñecaH);
        }

        private void btnMuñecaH_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(muñecaH);
        }

        private void btnGarraA_S_MouseDown(object sender, MouseEventArgs e)
        {
            keyDown(garraC);
        }

        private void btnGarraA_S_MouseUp(object sender, MouseEventArgs e)
        {
            keyUp(garraC);
        }

        private void nud_ValueChanged(MotorName motorName, NumericUpDown nudVelocidad)
        {
            if (nudVelocidad.Value > nudVelocidad.Maximum)
            {
                nudVelocidad.Value = nudVelocidad.Maximum;
            }
            else if (nudVelocidad.Value < nudVelocidad.Minimum)
            {
                nudVelocidad.Value = nudVelocidad.Minimum;
            }
            this.roboticArmModel.velUpdate(motorName, nudVelocidad.Value);
        }

        private void nudCodoMunieca_ValueChanged(object sender, EventArgs e)
        {
            nud_ValueChanged(MotorName.MUNIECA, nudCodoMunieca);
        }

        private void nudHombro_ValueChanged(object sender, EventArgs e)
        {
            nud_ValueChanged(MotorName.HOMBRO, nudHombro);
        }

        private void nudBase_ValueChanged(object sender, EventArgs e)
        {
            nud_ValueChanged(MotorName.BASE, nudBase);
        }


    }
}
