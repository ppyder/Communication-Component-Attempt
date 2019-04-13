using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO;
using System.Runtime.InteropServices;

namespace TempUpper
{
    public partial class MainForm : Form
    {
        //声明读写INI文件的API函数
        [DllImport("kernel32")]
        private static extern bool WritePrivateProfileString(string section, string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, byte[] retVal, int size, string filePath);

        UserDatas Datas = new UserDatas();

        //创建可以持续向文件写入的流工具
        StreamWriter SW;
        public bool isReadyToWrite = false;

        //通信组件
        public InterfaceToSlave Comunication = new InterfaceToSlave();

        public MainForm()
        {
            InitializeComponent();

            //初始化
            InitProgram();

            //实例化通信组件中的用户数据
            Comunication.InitUserDatas(ref Datas);
        }

        #region 读写配置文件

        //读取指定INI文件
        public string Read(string Section, string Ident, string Default)
        {
            string FileName; //INI文件名
            //加载全局变量
            FileName = Application.StartupPath + "\\Config.ini";
            Byte[] Buffer = new Byte[65535];
            int bufLen = GetPrivateProfileString(Section, Ident, Default, Buffer, Buffer.GetUpperBound(0), FileName);
            //必须设定0（系统默认的代码页）的编码方式，否则无法支持中文
            string s = Encoding.GetEncoding(0).GetString(Buffer);
            s = s.Substring(0, bufLen);
            return s.Trim();
        }

        //写INI文件
        public static void WriteIni(string Section, string Ident, string Value)
        {
            string FileName; //INI文件名
            //加载全局变量
            FileName = Application.StartupPath + "\\Config.ini";
            if (!WritePrivateProfileString(Section, Ident, Value, FileName))
            {

                throw (new ApplicationException("写入配置文件出错"));
            }

        }

        //预置通信参量
        private void ReadCOMData()
        {
            Comunication.SP_Baudrate = Read("BAUDRATE", "baudrate", "9600");
            Comunication.SP_DataBits = Read("DATABITS", "databits", "8");
            Comunication.SP_StopBits = Read("STOP", "stop", "1");
            Comunication.SP_Parity = Read("PARITY", "parity", "NONE");
        }

        //保存通信参量
        private void SaveCommand()
        {
            string FileName = Application.StartupPath + "\\Config.ini";

            Comunication.SP_Baudrate = cb_BaudRate.Text;
            Comunication.SP_DataBits = cbDataBits.Text;
            Comunication.SP_StopBits = cbStop.Text;
            Comunication.SP_Parity = cbParity.Text;

            //写入文件
            WriteIni("BAUDRATE", "baudrate", Comunication.SP_Baudrate);
            WriteIni("DATABITS", "databits", Comunication.SP_DataBits);
            WriteIni("STOP", "stop", Comunication.SP_StopBits);
            WriteIni("PARITY", "parity", Comunication.SP_Parity);
        }

        #endregion

        #region 保存上传的数据

        //开始保存数据前对文件路径的初始化工作
        private void FileInit()
        {
            //程序文件夹路径和保存文档所在路径
            string currPath = Application.StartupPath;
            string subPath = currPath + "\\testLog\\";

            //判断目标文件夹是否存在，如果不存在，就创建一个
            if (false == System.IO.Directory.Exists(subPath))
            {
                //创建pic文件夹
                System.IO.Directory.CreateDirectory(subPath);
            }

            //文件名
            string FileName = "DataLog";
            
            //在文件名中加入简报
            if(tb_Remark.Text == "(简报)")
            {
                FileName += DateTime.Now.ToString("yyyy-MM-dd hh_mm_ss") + ".txt";
            }
            else
            {
                 FileName += tb_Remark.Text + DateTime.Now.ToString("yyyy-MM-dd hh_mm_ss") + ".txt";
            }

            SW = System.IO.File.AppendText(subPath + FileName);

            //写入文件标头
            SW.WriteLine("At " + DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss") + " Save Data：");

            //写入数据标头,对应的两列数据的含义
            SW.WriteLine("Time_ms Input Output OutputUc");

            //写入文件
            SW.Flush();

            isReadyToWrite = true;

           // WriteinData("1 2 3 \n 4 5 6 \n 7 8 9");

        }

        //清除流中缓存的字符，并关闭文件
        private void AbortFileWriteIn()
        {
            //在数据记录文件尾部追加调试备注
            if(Richtb_Remark.Text != "(详情...)")
            {
                SW.WriteLine("本次调试备注：" + Richtb_Remark.Text);
            }

            SW.Flush();
            SW.Close();
        }

        //将已经处理好的字符串追加到文件中
        private void WriteinData(string Data)
        {
            if (isReadyToWrite)
            {
                SW.WriteLine(Data);

                //将刚刚写入缓冲区的行写入文件
                SW.Flush();
            }
            else
            {
                MessageBox.Show("尚未给出开始记录数据的指令，请先点击“开始记录”按钮！", "提示！");
            }
        }

        //向指定文件追加数据，一个DataArray是要写入的一行数据，中间以空格隔开，各个数据的含义将由标头指出
        public void WriteinData(double[] DataArray)
        {
            if (isReadyToWrite)
            {
                string DataToWriteIn = "";

                foreach (double Data in DataArray)
                {
                    DataToWriteIn += Data.ToString() + " ";
                }

                DataToWriteIn = DataToWriteIn.TrimEnd(' ');

                SW.WriteLine(DataToWriteIn);

                //将刚刚写入缓冲区的行写入文件
                SW.Flush();
            }
            else
            {
                MessageBox.Show("尚未给出开始记录数据的指令，请先点击“开始记录”按钮！", "提示！");
            }
        }
        #endregion

        #region 窗体事件

        #region 按钮
        //串口开关按钮
        private void btnSOpen_Click(object sender, EventArgs e)
        {
            //如果串口没有被打开
            if (!Comunication.State_Series.isopen)
            {
                try
                {
                    //打开串口
                    Comunication.OpenSerialPort(cbSerial.SelectedItem.ToString());

                    //使能发送定时器
                    SendTimer.Enabled = true;

                    //设置必要控件不可用
                    cbSerial.Enabled = false;
                    cb_BaudRate.Enabled = false;
                    cbDataBits.Enabled = false;
                    cbStop.Enabled = false;
                    cbParity.Enabled = false;

                    btnSOpen.Text = "关闭串口";
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("Error:" + ex.Message, "Error");

                    //恢复控件功能
                    cbSerial.Enabled = true;
                    cb_BaudRate.Enabled = true;
                    cbDataBits.Enabled = true;
                    cbStop.Enabled = true;
                    cbParity.Enabled = true;

                    btnSOpen.Text = "打开串口";

                    return;
                }
            }
            else
            {
                try
                {
                    Comunication.CloseSerialPort();

                    //使能发送定时器
                    SendTimer.Enabled = false;
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error!");

                    return;
                }

                //设置必要控件不可用
                cbSerial.Enabled = true;
                cb_BaudRate.Enabled = true;
                cbDataBits.Enabled = true;
                cbStop.Enabled = true;
                cbParity.Enabled = true;

                btnSOpen.Text = "打开串口";
            }
        }

        //启停记录按钮
        private void btn_record_Click(object sender, EventArgs e)
        {
            //如果没有开始记录
            if (!isReadyToWrite)
            {
                //初始化文件路径相关的信息
                FileInit();

                //发出计时器清零指令
                Comunication.Datas.Tx_TestCtrlCMD.CMD = 0x01;
                Comunication.SendRequest = UserDatas.MsgTypesID.Tx_SampleCMD;

                btn_record.Text = "停止记录";
            }
            else
            {
                //终止写入文件
                AbortFileWriteIn();

                //发出结束计时指令
                Comunication.Datas.Tx_TestCtrlCMD.CMD = 0x00;
                Comunication.SendRequest = UserDatas.MsgTypesID.Tx_SampleCMD;

                //标记当前不需要记录数据
                isReadyToWrite = false;

                btn_record.Text = "开始记录";
            }
        }

        //测试按钮：发送空指令
        private void btn_test_Click(object sender, EventArgs e)
        {
            //发出结束计时指令
            Comunication.Datas.Tx_TestCtrlCMD.CMD = 0x00;
            Comunication.SendRequest = UserDatas.MsgTypesID.Tx_SampleCMD;

        }

        //清空计数器按钮
        private void btn_ClearCounter_Click(object sender, EventArgs e)
        {
            //清零计数器
            Comunication.PackReceived = 0;
            Comunication.ByteReceived = 0;
            Comunication.ErrorPackReceived = 0;
        }

        //清空备注栏按钮
        private void btn_ClearRemark_Click(object sender, EventArgs e)
        {
            tb_Remark.Text = "(简报)";
            tb_Remark.ForeColor = SystemColors.WindowFrame;
            Richtb_Remark.Text = "(详情...)";
            Richtb_Remark.ForeColor = SystemColors.InactiveCaptionText;
        }
        #endregion

        #region 文本框功能实现及其辅助事件
        //简报内容改变时
        private void tb_Remark_TextChanged(object sender, EventArgs e)
        {
            tb_Remark.ForeColor = SystemColors.WindowText;
        }

        //详细描述变化时
        private void Richtb_Remark_TextChanged(object sender, EventArgs e)
        {
            Richtb_Remark.ForeColor = SystemColors.WindowText;
        }

        //简报文本框失去焦点时
        private void tb_Remark_Leave(object sender, EventArgs e)
        {
            //如果失去焦点时没有编辑过
            if (tb_Remark.Text == "")
            {
                tb_Remark.Text = "(简报)";
                tb_Remark.ForeColor = SystemColors.WindowFrame;
            }
        }

        //简报文本框获得焦点时
        private void tb_Remark_Enter(object sender, EventArgs e)
        {
            if (tb_Remark.Text == "(简报)")
            {
                tb_Remark.Text = "";
                tb_Remark.ForeColor = SystemColors.WindowText;
            }
        }

        //详细文本框失去焦点时
        private void Richtb_Remark_Leave(object sender, EventArgs e)
        {
            //如果失去焦点时没有编辑过
            if (Richtb_Remark.Text == "")
            {
                Richtb_Remark.Text = "(详情...)";
                Richtb_Remark.ForeColor = SystemColors.WindowFrame;
            }
        }

        //详细文本框获得焦点时
        private void Richtb_Remark_Enter(object sender, EventArgs e)
        {
            if (Richtb_Remark.Text == "(详情...)")
            {
                Richtb_Remark.Text = "";
                Richtb_Remark.ForeColor = SystemColors.WindowText;
            }
        }

        //点击窗体空白处时，设置焦点为某一个按钮
        private void MainForm_Click(object sender, EventArgs e)
        {
            btnSOpen.Focus();
        }
        #endregion

        #region 下拉框相关
        //点击串口下拉框时刷新串口列表
        private void cbSerial_MouseClick(object sender, MouseEventArgs e)
        {
            ScanCurrentSerialPort();
        }

        //更改波特率选择
        private void cb_BaudRate_SelectedIndexChanged(object sender, EventArgs e)
        {
            Comunication.SP_Baudrate = cb_BaudRate.Text;
        }

        //更改数据位选择
        private void cbDataBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            Comunication.SP_DataBits = cbDataBits.Text;
        }

        //更改停止位选择
        private void cbStop_SelectedIndexChanged(object sender, EventArgs e)
        {
            Comunication.SP_StopBits = cbStop.Text;
        }

        //更改校验选择
        private void cbParity_SelectedIndexChanged(object sender, EventArgs e)
        {
            Comunication.SP_Parity = cbParity.Text;
        }
        #endregion

        //窗体关闭时保存通信参量
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            SaveCommand();
        }

        //定时刷新参量
        private void timer_Tick(object sender, EventArgs e)
        {
            lb_PackReceivedNum.Text = Comunication.PackReceived.ToString() + "组";
            lb_ByteReceivedNum.Text = Comunication.ByteReceived.ToString() + "Byte(s)";
            lb_ErrorPackNum.Text = Comunication.ErrorPackReceived.ToString() + "包";
        }

        //5ms发送一次消息
        private void SendTimer_Tick(object sender, EventArgs e)
        {
            //5ms产生一个发送请求
            Comunication.SendData();
        }

        #endregion

        #region 数据更新和初始化操作

        private void DealSampleData(UserDatas.SampleData Data)
        {
            if (this.InvokeRequired)
            {
                //在此使窗体主线程异步地重新调用委托
                BeginInvoke(Datas.SampleDataHandler, Data);
            }
            else
            {
                //在此写入实际修改窗体控件的操作
                this.lb_TestKp.Text = Data.Time.ToString();
            }
        }

        //程序初始化时进行的操作
        private void InitProgram()
        {
            //扫描串口并更新下拉框
            ScanCurrentSerialPort();

            //实例化串口对象
            Comunication.InitSerialPort(ref serialPort);

            //预置通信参量
            ReadCOMData();

            //预置界面参数
            InitFormCBData();

            //初始化处理方法
            Datas.SampleHandlerInit(new UserDatas.SampleDataHandlerTypedef(DealSampleData));
        }

        //扫描电脑当前的串口并更新到下拉框中
        private void ScanCurrentSerialPort()
        {
            //检查是否含有串口
            string[] str = System.IO.Ports.SerialPort.GetPortNames();

            //清除原有项
            cbSerial.Items.Clear();

            if (str.Length != 0)                        //若有可用串口
            {
                //添加串口项目
                foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
                {
                    cbSerial.Items.Add(s);
                }

                //串口设置默认选择项
                cbSerial.SelectedIndex = 0;

            }
        }

        //预置下拉框参数
        private void InitFormCBData()
        {
            // 预置波特率
            switch (Comunication.SP_Baudrate)
            {
                case "300":
                    cb_BaudRate.SelectedIndex = 0;
                    break;
                case "600":
                    cb_BaudRate.SelectedIndex = 1;
                    break;
                case "1200":
                    cb_BaudRate.SelectedIndex = 2;
                    break;
                case "2400":
                    cb_BaudRate.SelectedIndex = 3;
                    break;
                case "4800":
                    cb_BaudRate.SelectedIndex = 4;
                    break;
                case "9600":
                    cb_BaudRate.SelectedIndex = 5;
                    break;
                case "19200":
                    cb_BaudRate.SelectedIndex = 6;
                    break;
                case "38400":
                    cb_BaudRate.SelectedIndex = 7;
                    break;
                case "115200":
                    cb_BaudRate.SelectedIndex = 8;
                    break;
                default:
                    {
                        MessageBox.Show("波特率预置参数错误！");
                        return;
                    }
            }

            //预置数据长度
            switch (Comunication.SP_DataBits)
            {
                case "5":
                    cbDataBits.SelectedIndex = 0;
                    break;
                case "6":
                    cbDataBits.SelectedIndex = 1;
                    break;
                case "7":
                    cbDataBits.SelectedIndex = 2;
                    break;
                case "8":
                    cbDataBits.SelectedIndex = 3;
                    break;
                default:
                    {
                        MessageBox.Show("数据位预置参数错误!");
                        return;
                    }

            }
            //预置停止位
            switch (Comunication.SP_StopBits)
            {
                case "1":
                    cbStop.SelectedIndex = 0;
                    break;
                case "1.5":
                    cbStop.SelectedIndex = 1;
                    break;
                case "2":
                    cbStop.SelectedIndex = 2;
                    break;
                default:
                    {
                        MessageBox.Show("停止位预置参数错误！");
                        return;
                    }
            }

            //预置校验位
            switch (Comunication.SP_Parity)
            {
                case "NONE":
                    cbParity.SelectedIndex = 0;
                    break;
                case "ODD":
                    cbParity.SelectedIndex = 1;
                    break;
                case "EVEN":
                    cbParity.SelectedIndex = 2;
                    break;
                default:
                    {
                        MessageBox.Show("校验位预置参数错误！");
                        return;
                    }
            }
        }

        #endregion
    }
}
