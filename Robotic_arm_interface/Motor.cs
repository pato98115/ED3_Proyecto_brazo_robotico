namespace Robotic_arm_interface
{
    internal class Motor
    {
        char stopCmd;
        char antiHCmd;
        char horaCmd;
        Estado estado;

        public Motor(char stopCmd, char antiHCmd, char horaCmd)
        {
            this.stopCmd = stopCmd;
            this.antiHCmd = antiHCmd;
            this.horaCmd = horaCmd;
            this.estado = Estado.DETENIDO;
        }

        public char getStopCmd()
        {
            return this.stopCmd;
        }
        public char getAntiHCmd()
        {
            return this.antiHCmd;
        }
        public char getHoraCmd()
        {
            return this.horaCmd;
        }
        public Estado getEstado()
        {
            return this.estado;
        }
        public void setEstado(Estado estado)
        {
            this.estado = estado;
        }
        

    }
}