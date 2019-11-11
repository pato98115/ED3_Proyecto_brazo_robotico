namespace Robotic_arm_interface
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panel2 = new System.Windows.Forms.Panel();
            this.rbtnEjecutar = new System.Windows.Forms.RadioButton();
            this.rbtnGrabar = new System.Windows.Forms.RadioButton();
            this.rbtnManual = new System.Windows.Forms.RadioButton();
            this.btnBaseA = new System.Windows.Forms.Button();
            this.btnHombroH = new System.Windows.Forms.Button();
            this.btnHombroA = new System.Windows.Forms.Button();
            this.btnBaseH = new System.Windows.Forms.Button();
            this.btnCodoH = new System.Windows.Forms.Button();
            this.btnCodoA = new System.Windows.Forms.Button();
            this.btnMuñecaH = new System.Windows.Forms.Button();
            this.btnMuñecaA = new System.Windows.Forms.Button();
            this.btnGarraA_S = new System.Windows.Forms.Button();
            this.lblTest = new System.Windows.Forms.Label();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.btnHelp = new System.Windows.Forms.Button();
            this.btnConectPort = new System.Windows.Forms.Button();
            this.rbtnManPote = new System.Windows.Forms.RadioButton();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.rbtnManPote);
            this.panel2.Controls.Add(this.rbtnEjecutar);
            this.panel2.Controls.Add(this.rbtnGrabar);
            this.panel2.Controls.Add(this.rbtnManual);
            this.panel2.Location = new System.Drawing.Point(35, 45);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(113, 93);
            this.panel2.TabIndex = 1;
            // 
            // rbtnEjecutar
            // 
            this.rbtnEjecutar.AutoSize = true;
            this.rbtnEjecutar.Location = new System.Drawing.Point(4, 51);
            this.rbtnEjecutar.Name = "rbtnEjecutar";
            this.rbtnEjecutar.Size = new System.Drawing.Size(64, 17);
            this.rbtnEjecutar.TabIndex = 2;
            this.rbtnEjecutar.TabStop = true;
            this.rbtnEjecutar.Text = "Ejecutar";
            this.rbtnEjecutar.UseVisualStyleBackColor = true;
            this.rbtnEjecutar.CheckedChanged += new System.EventHandler(this.rbtnEjecutar_CheckedChanged);
            // 
            // rbtnGrabar
            // 
            this.rbtnGrabar.AutoSize = true;
            this.rbtnGrabar.Location = new System.Drawing.Point(4, 28);
            this.rbtnGrabar.Name = "rbtnGrabar";
            this.rbtnGrabar.Size = new System.Drawing.Size(57, 17);
            this.rbtnGrabar.TabIndex = 1;
            this.rbtnGrabar.TabStop = true;
            this.rbtnGrabar.Text = "Grabar";
            this.rbtnGrabar.UseVisualStyleBackColor = true;
            this.rbtnGrabar.CheckedChanged += new System.EventHandler(this.rbtnGrabar_CheckedChanged);
            // 
            // rbtnManual
            // 
            this.rbtnManual.AutoSize = true;
            this.rbtnManual.Location = new System.Drawing.Point(4, 5);
            this.rbtnManual.Name = "rbtnManual";
            this.rbtnManual.Size = new System.Drawing.Size(60, 17);
            this.rbtnManual.TabIndex = 0;
            this.rbtnManual.TabStop = true;
            this.rbtnManual.Text = "Manual";
            this.rbtnManual.UseVisualStyleBackColor = true;
            this.rbtnManual.CheckedChanged += new System.EventHandler(this.rbtnManual_CheckedChanged);
            // 
            // btnBaseA
            // 
            this.btnBaseA.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBaseA.Location = new System.Drawing.Point(90, 144);
            this.btnBaseA.Name = "btnBaseA";
            this.btnBaseA.Size = new System.Drawing.Size(58, 53);
            this.btnBaseA.TabIndex = 0;
            this.btnBaseA.Text = "Base AntiH";
            this.btnBaseA.UseVisualStyleBackColor = true;
            this.btnBaseA.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnBaseA_MouseDown);
            this.btnBaseA.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnBaseA_MouseUp);
            // 
            // btnHombroH
            // 
            this.btnHombroH.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnHombroH.Location = new System.Drawing.Point(154, 144);
            this.btnHombroH.Name = "btnHombroH";
            this.btnHombroH.Size = new System.Drawing.Size(58, 53);
            this.btnHombroH.TabIndex = 1;
            this.btnHombroH.Text = "Hombro Hora";
            this.btnHombroH.UseVisualStyleBackColor = true;
            this.btnHombroH.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnHombroH_MouseDown);
            this.btnHombroH.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnHombroH_MouseUp);
            // 
            // btnHombroA
            // 
            this.btnHombroA.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnHombroA.Location = new System.Drawing.Point(154, 85);
            this.btnHombroA.Name = "btnHombroA";
            this.btnHombroA.Size = new System.Drawing.Size(58, 53);
            this.btnHombroA.TabIndex = 2;
            this.btnHombroA.Text = "Hombro AntiH";
            this.btnHombroA.UseVisualStyleBackColor = true;
            this.btnHombroA.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnHombroA_MouseDown);
            this.btnHombroA.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnHombroA_MouseUp);
            // 
            // btnBaseH
            // 
            this.btnBaseH.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBaseH.Location = new System.Drawing.Point(218, 144);
            this.btnBaseH.Name = "btnBaseH";
            this.btnBaseH.Size = new System.Drawing.Size(58, 53);
            this.btnBaseH.TabIndex = 3;
            this.btnBaseH.Text = "Base Hora";
            this.btnBaseH.UseVisualStyleBackColor = true;
            this.btnBaseH.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnBaseH_MouseDown);
            this.btnBaseH.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnBaseH_MouseUp);
            // 
            // btnCodoH
            // 
            this.btnCodoH.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCodoH.Location = new System.Drawing.Point(368, 144);
            this.btnCodoH.Name = "btnCodoH";
            this.btnCodoH.Size = new System.Drawing.Size(60, 53);
            this.btnCodoH.TabIndex = 4;
            this.btnCodoH.Text = "Codo Hora";
            this.btnCodoH.UseVisualStyleBackColor = true;
            this.btnCodoH.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnCodoH_MouseDown);
            this.btnCodoH.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnCodoH_MouseUp);
            // 
            // btnCodoA
            // 
            this.btnCodoA.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCodoA.Location = new System.Drawing.Point(368, 85);
            this.btnCodoA.Name = "btnCodoA";
            this.btnCodoA.Size = new System.Drawing.Size(60, 53);
            this.btnCodoA.TabIndex = 5;
            this.btnCodoA.Text = "Codo AntiH";
            this.btnCodoA.UseVisualStyleBackColor = true;
            this.btnCodoA.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnCodoA_MouseDown);
            this.btnCodoA.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnCodoA_MouseUp);
            // 
            // btnMuñecaH
            // 
            this.btnMuñecaH.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnMuñecaH.Location = new System.Drawing.Point(434, 144);
            this.btnMuñecaH.Name = "btnMuñecaH";
            this.btnMuñecaH.Size = new System.Drawing.Size(60, 53);
            this.btnMuñecaH.TabIndex = 6;
            this.btnMuñecaH.Text = "Muñeca Hora";
            this.btnMuñecaH.UseVisualStyleBackColor = true;
            this.btnMuñecaH.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnMuñecaH_MouseDown);
            this.btnMuñecaH.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnMuñecaH_MouseUp);
            // 
            // btnMuñecaA
            // 
            this.btnMuñecaA.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnMuñecaA.Location = new System.Drawing.Point(434, 85);
            this.btnMuñecaA.Name = "btnMuñecaA";
            this.btnMuñecaA.Size = new System.Drawing.Size(60, 53);
            this.btnMuñecaA.TabIndex = 7;
            this.btnMuñecaA.Text = "Muñeca AntiH";
            this.btnMuñecaA.UseVisualStyleBackColor = true;
            this.btnMuñecaA.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnMuñecaA_MouseDown);
            this.btnMuñecaA.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnMuñecaA_MouseUp);
            // 
            // btnGarraA_S
            // 
            this.btnGarraA_S.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnGarraA_S.Location = new System.Drawing.Point(179, 203);
            this.btnGarraA_S.Name = "btnGarraA_S";
            this.btnGarraA_S.Size = new System.Drawing.Size(170, 40);
            this.btnGarraA_S.TabIndex = 8;
            this.btnGarraA_S.Text = "Garra Agarrar/Soltar";
            this.btnGarraA_S.UseVisualStyleBackColor = true;
            this.btnGarraA_S.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnGarraA_S_MouseDown);
            this.btnGarraA_S.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnGarraA_S_MouseUp);
            // 
            // lblTest
            // 
            this.lblTest.AutoSize = true;
            this.lblTest.Location = new System.Drawing.Point(365, 9);
            this.lblTest.Name = "lblTest";
            this.lblTest.Size = new System.Drawing.Size(35, 13);
            this.lblTest.TabIndex = 9;
            this.lblTest.Text = "label1";
            // 
            // btnHelp
            // 
            this.btnHelp.Location = new System.Drawing.Point(525, 254);
            this.btnHelp.Name = "btnHelp";
            this.btnHelp.Size = new System.Drawing.Size(75, 23);
            this.btnHelp.TabIndex = 10;
            this.btnHelp.Text = "Help";
            this.btnHelp.UseVisualStyleBackColor = true;
            this.btnHelp.Click += new System.EventHandler(this.btnHelp_Click);
            // 
            // btnConectPort
            // 
            this.btnConectPort.Location = new System.Drawing.Point(35, 13);
            this.btnConectPort.Name = "btnConectPort";
            this.btnConectPort.Size = new System.Drawing.Size(113, 23);
            this.btnConectPort.TabIndex = 11;
            this.btnConectPort.Text = "Conectar";
            this.btnConectPort.UseVisualStyleBackColor = true;
            this.btnConectPort.Click += new System.EventHandler(this.btnConectPort_Click);
            // 
            // rbtnManPote
            // 
            this.rbtnManPote.AutoSize = true;
            this.rbtnManPote.Location = new System.Drawing.Point(4, 73);
            this.rbtnManPote.Name = "rbtnManPote";
            this.rbtnManPote.Size = new System.Drawing.Size(88, 17);
            this.rbtnManPote.TabIndex = 3;
            this.rbtnManPote.TabStop = true;
            this.rbtnManPote.Text = "Manual+Pote";
            this.rbtnManPote.UseVisualStyleBackColor = true;
            this.rbtnManPote.CheckedChanged += new System.EventHandler(this.rbtnManPote_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnablePreventFocusChange;
            this.ClientSize = new System.Drawing.Size(612, 289);
            this.Controls.Add(this.btnConectPort);
            this.Controls.Add(this.btnHelp);
            this.Controls.Add(this.lblTest);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.btnGarraA_S);
            this.Controls.Add(this.btnCodoH);
            this.Controls.Add(this.btnMuñecaH);
            this.Controls.Add(this.btnMuñecaA);
            this.Controls.Add(this.btnCodoA);
            this.Controls.Add(this.btnHombroA);
            this.Controls.Add(this.btnBaseH);
            this.Controls.Add(this.btnBaseA);
            this.Controls.Add(this.btnHombroH);
            this.Name = "Form1";
            this.Text = "Robotic Arm aplication";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RadioButton rbtnEjecutar;
        private System.Windows.Forms.RadioButton rbtnGrabar;
        private System.Windows.Forms.RadioButton rbtnManual;
        private System.Windows.Forms.Button btnBaseA;
        private System.Windows.Forms.Button btnHombroH;
        private System.Windows.Forms.Button btnHombroA;
        private System.Windows.Forms.Button btnBaseH;
        private System.Windows.Forms.Button btnCodoH;
        private System.Windows.Forms.Button btnCodoA;
        private System.Windows.Forms.Button btnMuñecaH;
        private System.Windows.Forms.Button btnMuñecaA;
        private System.Windows.Forms.Button btnGarraA_S;
        private System.Windows.Forms.Label lblTest;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button btnHelp;
        private System.Windows.Forms.Button btnConectPort;
        private System.Windows.Forms.RadioButton rbtnManPote;
    }
}

