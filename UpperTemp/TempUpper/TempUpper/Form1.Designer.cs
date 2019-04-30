namespace TempUpper
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnSOpen = new System.Windows.Forms.Button();
            this.cbSerial = new System.Windows.Forms.ComboBox();
            this.cbParity = new System.Windows.Forms.ComboBox();
            this.cbStop = new System.Windows.Forms.ComboBox();
            this.label35 = new System.Windows.Forms.Label();
            this.label36 = new System.Windows.Forms.Label();
            this.cbDataBits = new System.Windows.Forms.ComboBox();
            this.label37 = new System.Windows.Forms.Label();
            this.cbBaudRate = new System.Windows.Forms.ComboBox();
            this.label38 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.btn_record = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lb_PackReceivedNum = new System.Windows.Forms.Label();
            this.lb_ErrorPackNum = new System.Windows.Forms.Label();
            this.lb_ByteReceivedNum = new System.Windows.Forms.Label();
            this.cb_BaudRate = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.label6 = new System.Windows.Forms.Label();
            this.Richtb_Remark = new System.Windows.Forms.RichTextBox();
            this.btn_ClearCounter = new System.Windows.Forms.Button();
            this.btn_ClearRemark = new System.Windows.Forms.Button();
            this.tb_Remark = new System.Windows.Forms.TextBox();
            this.btn_test = new System.Windows.Forms.Button();
            this.SendTimer = new System.Windows.Forms.Timer(this.components);
            this.label7 = new System.Windows.Forms.Label();
            this.lb_TestKp = new System.Windows.Forms.Label();
            this.lb_RequestError = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.lb_testString = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnSOpen
            // 
            this.btnSOpen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSOpen.Location = new System.Drawing.Point(241, 13);
            this.btnSOpen.Name = "btnSOpen";
            this.btnSOpen.Size = new System.Drawing.Size(75, 23);
            this.btnSOpen.TabIndex = 3;
            this.btnSOpen.Text = "打开串口";
            this.btnSOpen.UseVisualStyleBackColor = true;
            this.btnSOpen.Click += new System.EventHandler(this.btnSOpen_Click);
            // 
            // cbSerial
            // 
            this.cbSerial.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbSerial.FormattingEnabled = true;
            this.cbSerial.Location = new System.Drawing.Point(84, 18);
            this.cbSerial.MaxDropDownItems = 10;
            this.cbSerial.Name = "cbSerial";
            this.cbSerial.Size = new System.Drawing.Size(75, 20);
            this.cbSerial.TabIndex = 2;
            this.cbSerial.MouseClick += new System.Windows.Forms.MouseEventHandler(this.cbSerial_MouseClick);
            // 
            // cbParity
            // 
            this.cbParity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbParity.FormattingEnabled = true;
            this.cbParity.Items.AddRange(new object[] {
            "无",
            "奇校验",
            "偶校验"});
            this.cbParity.Location = new System.Drawing.Point(84, 122);
            this.cbParity.Name = "cbParity";
            this.cbParity.Size = new System.Drawing.Size(75, 20);
            this.cbParity.TabIndex = 37;
            this.cbParity.SelectedIndexChanged += new System.EventHandler(this.cbParity_SelectedIndexChanged);
            // 
            // cbStop
            // 
            this.cbStop.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbStop.FormattingEnabled = true;
            this.cbStop.Items.AddRange(new object[] {
            "1",
            "1.5",
            "2"});
            this.cbStop.Location = new System.Drawing.Point(84, 96);
            this.cbStop.Name = "cbStop";
            this.cbStop.Size = new System.Drawing.Size(75, 20);
            this.cbStop.TabIndex = 36;
            this.cbStop.SelectedIndexChanged += new System.EventHandler(this.cbStop_SelectedIndexChanged);
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label35.Location = new System.Drawing.Point(15, 123);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(63, 14);
            this.label35.TabIndex = 34;
            this.label35.Text = "校验位：";
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label36.Location = new System.Drawing.Point(15, 97);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(63, 14);
            this.label36.TabIndex = 35;
            this.label36.Text = "停止位：";
            // 
            // cbDataBits
            // 
            this.cbDataBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbDataBits.FormattingEnabled = true;
            this.cbDataBits.Items.AddRange(new object[] {
            "5",
            "6",
            "7",
            "8"});
            this.cbDataBits.Location = new System.Drawing.Point(84, 70);
            this.cbDataBits.Name = "cbDataBits";
            this.cbDataBits.Size = new System.Drawing.Size(75, 20);
            this.cbDataBits.TabIndex = 33;
            this.cbDataBits.SelectedIndexChanged += new System.EventHandler(this.cbDataBits_SelectedIndexChanged);
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label37.Location = new System.Drawing.Point(15, 71);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(63, 14);
            this.label37.TabIndex = 32;
            this.label37.Text = "数据位：";
            // 
            // cbBaudRate
            // 
            this.cbBaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbBaudRate.FormattingEnabled = true;
            this.cbBaudRate.Items.AddRange(new object[] {
            "300",
            "600",
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "115200"});
            this.cbBaudRate.Location = new System.Drawing.Point(-106, 121);
            this.cbBaudRate.Name = "cbBaudRate";
            this.cbBaudRate.Size = new System.Drawing.Size(63, 20);
            this.cbBaudRate.TabIndex = 31;
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label38.Location = new System.Drawing.Point(-167, 122);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(56, 14);
            this.label38.TabIndex = 30;
            this.label38.Text = "波特率:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(15, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 14);
            this.label1.TabIndex = 38;
            this.label1.Text = "串口号：";
            // 
            // btn_record
            // 
            this.btn_record.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_record.Location = new System.Drawing.Point(241, 42);
            this.btn_record.Name = "btn_record";
            this.btn_record.Size = new System.Drawing.Size(75, 23);
            this.btn_record.TabIndex = 39;
            this.btn_record.Text = "开始记录";
            this.btn_record.UseVisualStyleBackColor = true;
            this.btn_record.Click += new System.EventHandler(this.btn_record_Click);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 205);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 12);
            this.label2.TabIndex = 40;
            this.label2.Text = "已接收数据：";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.ForeColor = System.Drawing.Color.Red;
            this.label3.Location = new System.Drawing.Point(17, 262);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(83, 12);
            this.label3.TabIndex = 41;
            this.label3.Text = "错误包数量：";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 234);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 12);
            this.label4.TabIndex = 42;
            this.label4.Text = "已记录数据：";
            // 
            // lb_PackReceivedNum
            // 
            this.lb_PackReceivedNum.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lb_PackReceivedNum.AutoSize = true;
            this.lb_PackReceivedNum.Location = new System.Drawing.Point(100, 234);
            this.lb_PackReceivedNum.Name = "lb_PackReceivedNum";
            this.lb_PackReceivedNum.Size = new System.Drawing.Size(35, 12);
            this.lb_PackReceivedNum.TabIndex = 43;
            this.lb_PackReceivedNum.Text = "0(包)";
            // 
            // lb_ErrorPackNum
            // 
            this.lb_ErrorPackNum.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lb_ErrorPackNum.AutoSize = true;
            this.lb_ErrorPackNum.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lb_ErrorPackNum.ForeColor = System.Drawing.Color.Red;
            this.lb_ErrorPackNum.Location = new System.Drawing.Point(100, 262);
            this.lb_ErrorPackNum.Name = "lb_ErrorPackNum";
            this.lb_ErrorPackNum.Size = new System.Drawing.Size(39, 12);
            this.lb_ErrorPackNum.TabIndex = 44;
            this.lb_ErrorPackNum.Text = "0(个)";
            // 
            // lb_ByteReceivedNum
            // 
            this.lb_ByteReceivedNum.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lb_ByteReceivedNum.AutoSize = true;
            this.lb_ByteReceivedNum.Location = new System.Drawing.Point(100, 205);
            this.lb_ByteReceivedNum.Name = "lb_ByteReceivedNum";
            this.lb_ByteReceivedNum.Size = new System.Drawing.Size(53, 12);
            this.lb_ByteReceivedNum.TabIndex = 45;
            this.lb_ByteReceivedNum.Text = "0Byte(s)";
            // 
            // cb_BaudRate
            // 
            this.cb_BaudRate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cb_BaudRate.FormattingEnabled = true;
            this.cb_BaudRate.Items.AddRange(new object[] {
            "300",
            "600",
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "115200"});
            this.cb_BaudRate.Location = new System.Drawing.Point(84, 44);
            this.cb_BaudRate.Name = "cb_BaudRate";
            this.cb_BaudRate.Size = new System.Drawing.Size(75, 20);
            this.cb_BaudRate.TabIndex = 47;
            this.cb_BaudRate.SelectedIndexChanged += new System.EventHandler(this.cb_BaudRate_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(15, 45);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(56, 14);
            this.label5.TabIndex = 46;
            this.label5.Text = "波特率:";
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Interval = 200;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(203, 103);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(113, 12);
            this.label6.TabIndex = 48;
            this.label6.Text = "对本次调试的备注：";
            // 
            // Richtb_Remark
            // 
            this.Richtb_Remark.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.Richtb_Remark.ForeColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.Richtb_Remark.Location = new System.Drawing.Point(205, 150);
            this.Richtb_Remark.Name = "Richtb_Remark";
            this.Richtb_Remark.Size = new System.Drawing.Size(142, 103);
            this.Richtb_Remark.TabIndex = 49;
            this.Richtb_Remark.Text = "(详情...)";
            this.Richtb_Remark.TextChanged += new System.EventHandler(this.Richtb_Remark_TextChanged);
            this.Richtb_Remark.Enter += new System.EventHandler(this.Richtb_Remark_Enter);
            this.Richtb_Remark.Leave += new System.EventHandler(this.Richtb_Remark_Leave);
            // 
            // btn_ClearCounter
            // 
            this.btn_ClearCounter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btn_ClearCounter.Location = new System.Drawing.Point(145, 257);
            this.btn_ClearCounter.Name = "btn_ClearCounter";
            this.btn_ClearCounter.Size = new System.Drawing.Size(45, 23);
            this.btn_ClearCounter.TabIndex = 50;
            this.btn_ClearCounter.Text = "清零";
            this.btn_ClearCounter.UseVisualStyleBackColor = true;
            this.btn_ClearCounter.Click += new System.EventHandler(this.btn_ClearCounter_Click);
            // 
            // btn_ClearRemark
            // 
            this.btn_ClearRemark.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_ClearRemark.Location = new System.Drawing.Point(302, 257);
            this.btn_ClearRemark.Name = "btn_ClearRemark";
            this.btn_ClearRemark.Size = new System.Drawing.Size(45, 23);
            this.btn_ClearRemark.TabIndex = 51;
            this.btn_ClearRemark.Text = "清空";
            this.btn_ClearRemark.UseVisualStyleBackColor = true;
            this.btn_ClearRemark.Click += new System.EventHandler(this.btn_ClearRemark_Click);
            // 
            // tb_Remark
            // 
            this.tb_Remark.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tb_Remark.ForeColor = System.Drawing.SystemColors.WindowFrame;
            this.tb_Remark.Location = new System.Drawing.Point(205, 123);
            this.tb_Remark.Name = "tb_Remark";
            this.tb_Remark.Size = new System.Drawing.Size(142, 21);
            this.tb_Remark.TabIndex = 52;
            this.tb_Remark.Text = "(简报)";
            this.tb_Remark.TextChanged += new System.EventHandler(this.tb_Remark_TextChanged);
            this.tb_Remark.Enter += new System.EventHandler(this.tb_Remark_Enter);
            this.tb_Remark.Leave += new System.EventHandler(this.tb_Remark_Leave);
            // 
            // btn_test
            // 
            this.btn_test.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_test.Location = new System.Drawing.Point(241, 71);
            this.btn_test.Name = "btn_test";
            this.btn_test.Size = new System.Drawing.Size(75, 23);
            this.btn_test.TabIndex = 53;
            this.btn_test.Text = "指令测试";
            this.btn_test.UseVisualStyleBackColor = true;
            this.btn_test.Click += new System.EventHandler(this.btn_test_Click);
            // 
            // SendTimer
            // 
            this.SendTimer.Interval = 1;
            this.SendTimer.Tick += new System.EventHandler(this.SendTimer_Tick);
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(25, 289);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 12);
            this.label7.TabIndex = 54;
            this.label7.Text = "Test Kp:";
            // 
            // lb_TestKp
            // 
            this.lb_TestKp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lb_TestKp.AutoSize = true;
            this.lb_TestKp.Location = new System.Drawing.Point(84, 289);
            this.lb_TestKp.Name = "lb_TestKp";
            this.lb_TestKp.Size = new System.Drawing.Size(53, 12);
            this.lb_TestKp.TabIndex = 55;
            this.lb_TestKp.Text = "0Byte(s)";
            // 
            // lb_RequestError
            // 
            this.lb_RequestError.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lb_RequestError.AutoSize = true;
            this.lb_RequestError.Location = new System.Drawing.Point(272, 289);
            this.lb_RequestError.Name = "lb_RequestError";
            this.lb_RequestError.Size = new System.Drawing.Size(53, 12);
            this.lb_RequestError.TabIndex = 57;
            this.lb_RequestError.Text = "0Byte(s)";
            // 
            // label9
            // 
            this.label9.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(165, 289);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(101, 12);
            this.label9.TabIndex = 56;
            this.label9.Text = "RequestErrorCnt:";
            // 
            // lb_testString
            // 
            this.lb_testString.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lb_testString.AutoSize = true;
            this.lb_testString.Location = new System.Drawing.Point(17, 167);
            this.lb_testString.Name = "lb_testString";
            this.lb_testString.Size = new System.Drawing.Size(65, 12);
            this.lb_testString.TabIndex = 58;
            this.lb_testString.Text = "TestString";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(359, 310);
            this.Controls.Add(this.lb_testString);
            this.Controls.Add(this.lb_RequestError);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.lb_TestKp);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.btn_test);
            this.Controls.Add(this.tb_Remark);
            this.Controls.Add(this.btn_ClearRemark);
            this.Controls.Add(this.btn_ClearCounter);
            this.Controls.Add(this.Richtb_Remark);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.cb_BaudRate);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.lb_ByteReceivedNum);
            this.Controls.Add(this.lb_ErrorPackNum);
            this.Controls.Add(this.lb_PackReceivedNum);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btn_record);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cbParity);
            this.Controls.Add(this.cbStop);
            this.Controls.Add(this.label35);
            this.Controls.Add(this.label36);
            this.Controls.Add(this.cbDataBits);
            this.Controls.Add(this.label37);
            this.Controls.Add(this.cbBaudRate);
            this.Controls.Add(this.label38);
            this.Controls.Add(this.btnSOpen);
            this.Controls.Add(this.cbSerial);
            this.Name = "MainForm";
            this.Text = "控制器调试助手";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Click += new System.EventHandler(this.MainForm_Click);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnSOpen;
        private System.Windows.Forms.ComboBox cbSerial;
        private System.Windows.Forms.ComboBox cbParity;
        private System.Windows.Forms.ComboBox cbStop;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.ComboBox cbDataBits;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.ComboBox cbBaudRate;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.Label label1;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Button btn_record;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lb_PackReceivedNum;
        private System.Windows.Forms.Label lb_ErrorPackNum;
        private System.Windows.Forms.Label lb_ByteReceivedNum;
        private System.Windows.Forms.ComboBox cb_BaudRate;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.RichTextBox Richtb_Remark;
        private System.Windows.Forms.Button btn_ClearCounter;
        private System.Windows.Forms.Button btn_ClearRemark;
        private System.Windows.Forms.TextBox tb_Remark;
        private System.Windows.Forms.Button btn_test;
        private System.Windows.Forms.Timer SendTimer;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label lb_TestKp;
        private System.Windows.Forms.Label lb_RequestError;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label lb_testString;
    }
}

