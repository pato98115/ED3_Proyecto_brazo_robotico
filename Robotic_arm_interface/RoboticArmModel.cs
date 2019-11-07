using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Robotic_arm_interface
{
    class RoboticArmModel
    {
        Motor Base;
        Motor Hombro;
        Motor Codo;
        Motor Muñeca;
        char garraChar;
        List<char> cmdsPress;
        Dictionary<ArmState, char> cmdsStatesPairs;
        ArmState armState;

        Form1 interfaz;

        public RoboticArmModel(Form1 interfaz)
        {
            this.interfaz = interfaz;
            createMotors();
            this.cmdsPress = new List<char>();
            setCmdsStates();
            this.setState(ArmState.MANUAL);
        }

        private void setCmdsStates()
        {
            this.cmdsStatesPairs = new Dictionary<ArmState, char>();
            cmdsStatesPairs.Add(ArmState.MANUAL, 'n');
            cmdsStatesPairs.Add(ArmState.GRABACION, 'o');
            cmdsStatesPairs.Add(ArmState.EJECUCION, 'p');
        }

        public void createMotors()
        {
            this.Base = new Motor('a', 'b', 'c');
            this.Hombro = new Motor('d', 'e', 'f');
            this.Codo = new Motor('g', 'h', 'i');
            this.Muñeca = new Motor('j', 'k', 'l');
            this.garraChar = 'm';
        }

        public void keyDown(MotorName motorName, KeyType keyType)
        {
            Motor motor = this.parseMotor(motorName);
            char cmd = '1';

            switch (keyType)
            {
      
                case KeyType.ANTIHORARIA:
                    if(motor.getEstado() != Estado.ANTIHORARIO)
                    {
                        cmd = motor.getAntiHCmd();
                        motor.setEstado(Estado.ANTIHORARIO);
                        this.interfaz.serialPortWrite(cmd);
                        this.cmdsPress.Add(cmd);
                    }                 
                    break;
                case KeyType.HORARIA:
                    if (motor.getEstado() != Estado.HORARIO)
                    {
                        cmd = motor.getHoraCmd();
                        motor.setEstado(Estado.HORARIO);
                        this.interfaz.serialPortWrite(cmd);
                        this.cmdsPress.Add(cmd);
                    }
                    break;
            }
            
        }

        public void keyUp(MotorName motorName, KeyType keyType)
        {
            Motor motor = this.parseMotor(motorName);
            char cmd = '1';

            switch (keyType)
            {
                case KeyType.ANTIHORARIA:
                    cmdsPress.Remove(motor.getAntiHCmd());
                    if (this.cmdsPress.Contains(motor.getHoraCmd()))
                    {
                        cmd = motor.getHoraCmd();
                        motor.setEstado(Estado.HORARIO);
                    }
                    else
                    {
                        cmd = motor.getStopCmd();
                        motor.setEstado(Estado.DETENIDO);
                    }
                    break;
                case KeyType.HORARIA:
                    cmdsPress.Remove(motor.getHoraCmd());
                    if (this.cmdsPress.Contains(motor.getAntiHCmd()))
                    {
                        cmd = motor.getAntiHCmd();
                        motor.setEstado(Estado.ANTIHORARIO);
                    }
                    else
                    {
                        cmd = motor.getStopCmd();
                        motor.setEstado(Estado.DETENIDO);
                    }
                    break;
            }
            this.interfaz.serialPortWrite(cmd);
        }


        private Motor parseMotor(MotorName motorName)
        {
            switch (motorName)
            {
                case MotorName.BASE:
                    return this.Base;
                case MotorName.HOMBRO:
                    return this.Hombro;
                case MotorName.CODO:
                    return this.Codo;
                case MotorName.MUNIECA:
                    return this.Muñeca;
                default:
                    return new Motor('2', '3', '4'); //error

            }
        }

        private Color getPressingColor()
        {
            switch (this.armState)
            {
                case ArmState.MANUAL:
                    return Color.LightGreen;
                case ArmState.GRABACION:
                    return Color.LightSalmon;
                case ArmState.EJECUCION:
                    return Color.LightGray;
                default:
                    return Color.DarkGray;
            }
        }

        public void setState(ArmState state)
        {
            this.armState = state;
            this.interfaz.setColorPressing(getPressingColor());
        }

        internal void manual()
        {
            this.setState(ArmState.MANUAL);
            char cmd;
            this.cmdsStatesPairs.TryGetValue(ArmState.MANUAL, out cmd);
            interfaz.serialPortWrite(cmd);

        }

        internal void grabar()
        {
            this.setState(ArmState.GRABACION);
            char cmd;
            this.cmdsStatesPairs.TryGetValue(ArmState.GRABACION, out cmd);
            interfaz.serialPortWrite(cmd);
        }

        internal void ejecutar()
        {
            this.setState(ArmState.EJECUCION);
            char cmd;
            this.cmdsStatesPairs.TryGetValue(ArmState.EJECUCION, out cmd);
            interfaz.serialPortWrite(cmd);
        }

        internal void garraCmd()
        {
            interfaz.serialPortWrite(this.garraChar);
        }
    }
}
