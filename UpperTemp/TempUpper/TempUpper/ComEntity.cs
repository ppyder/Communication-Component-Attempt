using System;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;

namespace TempUpper
{
    //通信实体组件
    public class ComEntity
    {
        #region 串口块

        //串口引用
        SerialPort sp;

        //串口状态
        flag Series;
        bool IsSerialInited;

        //字段参数，分别是：波特率、数据位、停止位、校验位（串口参数）
        private string iG_BAUDRATE, iG_DATABITS, iG_STOP, iG_PARITY;

        //状态属性：串口被选中的状态以及被打开的状态
        public flag State_Series
        {
            get
            {
                return Series;
            }
        }

        //设置属性：串口号
        public string SP_PortName
        {
            get
            {
                return sp.PortName;
            }
            set
            {
                //如果串口已打开
                if (sp.IsOpen)
                {
                    throw new Exception("<串口号>:设置失败！请先关闭串口再重试！");
                }
                else
                {
                    sp.PortName = value;
                }
            }
        }

        //设置属性：波特率(SP: serial port)
        public string SP_Baudrate
        {
            get
            {
                return iG_BAUDRATE;
            }
            set
            {
                switch (value)
                {
                    case "300":
                    case "600":
                    case "1200":
                    case "2400":
                    case "4800":
                    case "9600":
                    case "19200":
                    case "38400":
                    case "115200":
                        iG_BAUDRATE = value;
                        break;

                    default:
                        {
                            throw new Exception("<波特率>:错误的设置！\r\n本次设置值为：" + value);
                        }
                }

            }
        }

        //设置属性：数据位
        public string SP_DataBits
        {
            get
            {
                return iG_DATABITS;
            }
            set
            {
                switch (value)
                {
                    case "5":
                    case "6":
                    case "7":
                    case "8":
                        iG_DATABITS = value;
                        break;
                    default:
                        {
                            throw new Exception("<数据位>:错误的设置！\r\n本次设置值为：" + value);
                        }

                }

            }
        }

        //设置属性：停止位
        public string SP_StopBits
        {
            get
            {
                return iG_STOP;
            }
            set
            {
                switch (value)
                {
                    case "1":
                    case "1.5":
                    case "2":
                        iG_STOP = value;
                        break;
                    default:
                        {
                            throw new Exception("<停止位>:错误的设置！\r\n本次设置值为：" + value);
                        }
                }

            }
        }

        //设置属性：校验位
        public string SP_Parity
        {
            get
            {
                return iG_PARITY;
            }
            set
            {
                switch (value)
                {
                    case "无":
                        iG_PARITY = "NONE";
                        break;
                    case "奇校验":
                        iG_PARITY = "ODD";
                        break;
                    case "偶校验":
                        iG_PARITY = "EVEN";
                        break;

                    case "NONE":
                    case "ODD":
                    case "EVEN":
                        iG_PARITY = value;
                        break;

                    default:
                        {
                            throw new Exception("<校验位>:错误的设置！\r\n本次设置值为：" + value);
                        }
                }

            }
        }

        //实例化串口
        public void InitSerialPort(ref SerialPort NewPort)
        {
            //实例化串口
            sp = NewPort;

            sp.BaudRate = 9600;

            /* 关于注释：采用的是线程方式接收，因为事件方式接收会出现数据包被斩断的情况，所以就没有用 */
            ////添加串口接收事件
            //sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);

            //设定只要有一个字节接收就触发接收中断
            sp.ReceivedBytesThreshold = 1;

            //准备就绪              
            sp.DtrEnable = true;
            sp.RtsEnable = true;

            //设置数据读取超时为1秒
            sp.ReadTimeout = 1000;

            sp.Close();

            IsSerialInited = true;
        }

        //打开串口(参数为串口号)
        public void OpenSerialPort(string SerialName)
        {
            //如果还没有初始化串口对象
            if (!IsSerialInited)
            {
                throw new Exception("<打开串口>：串口对象尚未被初始化，操作无效！");
            }

            if (!sp.IsOpen)
            {
                try
                {
                    //设置串口号
                    sp.PortName = SerialName;

                    //设置各“串口设置”
                    string strBaudRate = iG_BAUDRATE;
                    string strDateBits = iG_DATABITS; ;
                    string strStopBits = iG_STOP;
                    Int32 iBaudRate = Convert.ToInt32(strBaudRate);
                    Int32 iDateBits = Convert.ToInt32(strDateBits);

                    sp.BaudRate = iBaudRate;       //波特率
                    sp.DataBits = iDateBits;       //数据位

                    switch (iG_STOP)            //停止位
                    {
                        case "1":
                            sp.StopBits = StopBits.One;
                            break;
                        case "1.5":
                            sp.StopBits = StopBits.OnePointFive;
                            break;
                        case "2":
                            sp.StopBits = StopBits.Two;
                            break;
                        default:
                            throw new Exception("<打开串口>: 停止位参数不正确!");
                    }
                    switch (iG_PARITY)             //校验位
                    {
                        case "NONE":
                            sp.Parity = Parity.None;
                            break;
                        case "ODD":
                            sp.Parity = Parity.Odd;
                            break;
                        case "EVEN":
                            sp.Parity = Parity.Even;
                            break;
                        default:
                            throw new Exception("<打开串口>: 校验位参数不正确!");
                    }

                    sp.Open();     //打开串口
                    Series.isopen = true;

                    //开启线程
                    th = new Thread(SpReceiveMsg);
                    th.IsBackground = true;
                    th.Start();
                }
                catch (System.Exception ex)
                {
                    Series.isopen = false;

                    //抛出异常
                    throw new Exception("<打开串口>: 打开串口失败！\r\n", ex);
                }

            }
        }

        //关闭串口
        public void CloseSerialPort()
        {
            try
            {
                //终止线程
                th.Abort();

                sp.Close();

                Series.isopen = false;
            }
            catch (System.Exception ex)
            {
                throw new Exception("ComEntity: 串口关闭失败", ex);
            }
        }

        //串口发送
        void Series_SendData(byte[] TxBuffer)
        {

            if (!sp.IsOpen) //如果没打开
            {
                throw new Exception("<串口发送>：未连接到对方！");
            }
            else
            {
                sp.Write(TxBuffer, 0, TxBuffer.Length);    //写入发送缓冲区
            }

        }

        //线程实现：串口接收
        void SpReceiveMsg()
        {
            while (true)
            {
                try
                {
                    if (sp.BytesToRead > 0)
                    {
                        int temp = sp.ReadByte();
                        byte[] buffer = new byte[1];

                        if (-1 != temp)
                        {
                            buffer[0] = (byte)temp;

                            //处理接收的数据
                            ReceiveData(buffer);
                        }
                    }

                }
                catch (Exception ex)
                {
                    SaveInfo("<串口接收>:" + ex.Message + "\r\n");
                    //th.Join();
                }

            }
        }
        #endregion

        #region 网口块

        //网络引用
        Socket socketControl;

        //网口状态标志
        flag Net;

        //字段参数，分别是：视频地址、链接地址、控制端口（网口参数）
        private string iVideoAdd, iConnectAdd, iCtrlport;

        //状态属性：网口被选中的状态以及被打开的状态
        public flag State_Net
        {
            get
            {
                return Net;
            }
        }

        //设置属性：链接IP
        public string Net_ConnectAddress
        {
            get
            {
                return iConnectAdd;
            }
            set
            {
                iConnectAdd = value;
            }
        }

        //设置属性：控制端口
        public string Net_ControlPort
        {
            get
            {
                return iCtrlport;
            }
            set
            {
                iCtrlport = value;
            }
        }

        //设置属性：视频地址
        public string Net_VideoAddress
        {
            get
            {
                return iVideoAdd;
            }
            set
            {
                iVideoAdd = value;
            }
        }

        //打开网口(建立与从机的链接并准备接收)
        public void Net_Connect()
        {
            IPAddress ips = IPAddress.Parse(iConnectAdd.ToString());//("192.168.2.1");
            IPEndPoint ipe = new IPEndPoint(ips, Convert.ToInt32(iCtrlport.ToString()));//把ip和端口转化为IPEndPoint实例
            socketControl = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                socketControl.Connect(ipe);//连接到服务器                      
            }
            catch (Exception ex)
            {
                throw new Exception("<打开网口>: 网口连接失败！", ex);
            }

            //更新标志
            Net.isopen = true;

            //开启线程
            th = new Thread(NetReceiveMsg);
            th.IsBackground = true;
            th.Start();
        }

        //关闭网口
        public void Net_disconnect()
        {
            //终止线程
            th.Abort();

            //关闭Socket链接
            socketControl.Close();

            //更新状态标志
            Net.isopen = false;
        }

        //网口发送数据
        private void NetSendData(byte[] TxBuffer)
        {
            if (Net.isopen == false)
            {
                throw new Exception("<网口发送>：未连接到对方！");
            }
            else
            {
                try
                {
                    socketControl.Send(TxBuffer, TxBuffer.Length, 0);
                    //socketControl.Close();
                }
                catch (Exception e)
                {
                    Net_disconnect();
                    throw new Exception("<网口发送>:", e);

                }
            }

        }

        //线程实现：网口接收回传数据
        private void NetReceiveMsg()
        {
            while (true)
            {
                try
                {
                    byte[] TempBuffer = new byte[socketControl.ReceiveBufferSize];
                    int n = socketControl.Receive(TempBuffer);

                    byte[] Buffer = new byte[n];

                    //获得实际数据缓冲区
                    for (int i = 0; i < n; i++)
                    {
                        Buffer[i] = TempBuffer[i];
                    }

                    //处理接收的数据
                    ReceiveData(Buffer);

                }
                catch (Exception ex)
                {
                    SaveInfo("<网口接收>:" + ex.Message);
                    th.Join();
                    break;
                }

            }
        }

        //加载指定视频地址的图像到特定的pictureBox
        public void LoadPicture(ref PictureBox TargetPictureBox)
        {
            try
            {
                TargetPictureBox.ImageLocation = iVideoAdd;
                TargetPictureBox.LoadAsync();
            }
            catch (System.Exception ex)
            {
                TargetPictureBox.CancelAsync();
                throw new Exception("<加载图片>:", ex);

            }
        }
        #endregion

        #region 通用块

        //通信实体的状态标识
        public struct flag
        {
            public bool isopen;
            public bool ischecked;
        }

        //通信载体标识，用于选择通信载体
        public enum Info_ChannelTypes
        {
            channel_null = 0x00,        //标识一种未选择的状态
            channel_Serial = 0x01,
            channal_Net = 0x02,
        }

        //线程引用
        Thread th;

        //文本缓冲区
        private string InfoBuffer = "";
        private int InfoBufferLength = 20000; //文本缓冲区的大小（字符个数）

        //设置属性：通知数据缓冲区大小，单位为字符个数
        public int InfoBufferSize
        {
            get
            {
                return InfoBufferLength;
            }
            set
            {
                InfoBufferLength = value;
            }
        }

        //内容属性：文本缓冲区(信息接收时，用于存储非致命通知的缓冲区,读取后自动清空)
        public string InfoStrBuffer
        {
            get
            {
                string temp = InfoBuffer;

                InfoBuffer = "";

                return temp;
            }
        }

        //动作属性:获取文本缓冲区的内容（受保护）
        protected string InfoBuffer_Read
        {
            get
            {
                return InfoBuffer;
            }
        }

        //设置属性：设置通信载体
        public Info_ChannelTypes Info_Channel
        {
            get
            {
                if (Net.ischecked)
                {
                    return Info_ChannelTypes.channal_Net;
                }
                else if (Series.ischecked)
                {
                    return Info_ChannelTypes.channel_Serial;
                }
                else
                {
                    return Info_ChannelTypes.channel_null;
                }
            }
            set
            {
                //如果有信道已经打开
                if (Net.isopen || Series.isopen)
                {
                    throw new Exception("<信道选择>: 选择失败！请先关闭当前信道再尝试！");
                }

                switch (value)
                {
                    case Info_ChannelTypes.channal_Net:

                        Net.ischecked = true;
                        Series.ischecked = false;
                        break;

                    case Info_ChannelTypes.channel_Serial:

                        Net.ischecked = false;
                        Series.ischecked = true;
                        break;

                    case Info_ChannelTypes.channel_null:

                        Net.ischecked = false;
                        Series.ischecked = false;
                        break;

                    default:
                        throw new Exception("<信道选择>: 非法的信道选择！");
                }
            }
        }

        //将产生的非致命通知存入文本缓冲区中(致命通知以异常的形式抛出)
        protected void SaveInfo(string Str)
        {
            //输出当前时间
            DateTime dt = DateTime.Now;
            string str_Time = dt.GetDateTimeFormats('f')[0].ToString() + "\r\n";

            //如果存入了太多的字符串而没有清理
            if (InfoBuffer.Length > InfoBufferLength)
                InfoBuffer = "";

            InfoBuffer += (str_Time + Str);
        }

        //处理数据的虚函数，由继承类实现
        protected virtual void ReceiveData(byte[] Buffer) { }
        protected virtual byte[] DealDataToSend() { return null; }

        //通信实体构造函数
        public ComEntity()
        {
            //初始化状态标识
            IsSerialInited = false;

            Series.isopen = false;
            Series.ischecked = true;
            Net.ischecked = false;
            Net.isopen = false;

            return;
        }

        //发送指令
        public void SendData()
        {
            byte[] DataToSend = DealDataToSend();

            //如果待发送的数据缓冲区为空则不进行发送操作
            if (null == DataToSend || 0 == DataToSend.Length)
            {
                return;
            }

            if (Series.isopen == true)
            {
                try
                {
                    Series_SendData(DataToSend);
                }
                catch (System.Exception ex)
                {
                    throw new Exception("<串口发送>:Send Error!\n", ex);
                }
            }
            else if (Net.isopen == true)
            {
                try
                {
                    NetSendData(DataToSend);
                }
                catch (System.Exception ex)
                {
                    throw new Exception("<网口发送>:Send Error!\n", ex);
                }
            }
            else
            {
                throw new Exception("<发送>未连接下位机！");
            }
        }
        public void SendData(byte[] SendBuffer)
        {
            //如果输入的发送缓冲区为空则报错
            if (null == SendBuffer || 0 == SendBuffer.Length)
            {
                throw new Exception("<数据发送>:传入发送函数的数据缓冲区为空!\n");
            }

            if (Series.isopen == true)
            {
                try
                {
                    Series_SendData(SendBuffer);
                }
                catch (System.Exception ex)
                {
                    throw new Exception("<串口发送>:Send Error!\n", ex);
                }
            }
            else if (Net.isopen == true)
            {
                try
                {
                    NetSendData(SendBuffer);
                }
                catch (System.Exception ex)
                {
                    throw new Exception("<网口发送>:Send Error!\n", ex);
                }
            }
            else
            {
                throw new Exception("<发送>未连接下位机！");
            }
        }
        #endregion
    }
}
