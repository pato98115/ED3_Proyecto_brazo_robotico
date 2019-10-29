namespace Robotic_arm_interface
{
    partial class FormConectMsg
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lblControlHelp = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // lblControlHelp
            // 
            this.lblControlHelp.AutoSize = true;
            this.lblControlHelp.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblControlHelp.Location = new System.Drawing.Point(34, 33);
            this.lblControlHelp.Name = "lblControlHelp";
            this.lblControlHelp.Size = new System.Drawing.Size(320, 100);
            this.lblControlHelp.TabIndex = 0;
            this.lblControlHelp.Text = "PROBLEMAS CON LA CONCECCION \r\nCON EL PUERTO SERIE.\r\nASEGURECE DE CONFIGURARLO Y \r" +
    "\nCONECTARLO ANTES DE INTENTAR\r\nREALIZAR UNA COMUNICACION";
            // 
            // FormConectMsg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(390, 217);
            this.Controls.Add(this.lblControlHelp);
            this.Name = "FormConectMsg";
            this.Text = "Conection Warning";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblControlHelp;
    }
}