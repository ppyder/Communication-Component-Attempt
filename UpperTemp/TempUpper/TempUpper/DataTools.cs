using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;   //Marshal.SizeOf用到
using System.Threading.Tasks;

namespace TempUpper
{
    //数据处理工具
    public class DataTools
    {
        public DataTools()
        {
            return;
        }

        ~DataTools()
        {
            return;
        }

        #region 数据转换方法

        //struct转换为Byte[]
        public Byte[] StructToBytes(Object Data)
        {
            Int32 size = Marshal.SizeOf(Data);
            IntPtr buffer = Marshal.AllocHGlobal(size);

            try
            {
                Marshal.StructureToPtr(Data, buffer, false);
                Byte[] bytes = new Byte[size];
                Marshal.Copy(buffer, bytes, 0, size);

                return bytes;
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }

        }
        public void StructToBytes(Object Data, ref Byte[] bytes, int StartIndex)
        {
            Int32 size = Marshal.SizeOf(Data);
            IntPtr buffer = Marshal.AllocHGlobal(size);

            if ((StartIndex + size) > bytes.Length)
            {
                Marshal.FreeHGlobal(buffer);
                throw new Exception("<struct转Byte[]>:输入索引与待转换结构体长度之和超出数组范围！");
            }

            try
            {
                Marshal.StructureToPtr(Data, buffer, false);
                Marshal.Copy(buffer, bytes, StartIndex, size);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }

        }

        /* Byte[]转换为struct
         * 
         * 参数：
         *      bytes:传入的待转换的数组
         *      StartIndex:数据起始位置索引
         *      strcutType：目标结构体类型
         *      
         * 返回：目标结构体实例，转换后的结果
         */
        public Object BytesToStruct(Byte[] bytes, int StartIndex, Type strcutType)
        {
            int size = Marshal.SizeOf(strcutType);
            IntPtr buffer = Marshal.AllocHGlobal(size);

            if (bytes.Length < (StartIndex + size))  //如果待转换的目标内存不完整或不存在
            {
                throw new Exception("<DataTool>" + strcutType.ToString() + ":待转换的目标内存不完整或不存在。\r\n" + "数据包转换时出错！");
            }

            try
            {
                //将数组中的前size位拷贝进目标类型中
                Marshal.Copy(bytes, StartIndex, buffer, size);

                return Marshal.PtrToStructure(buffer, strcutType);
            }
            catch (System.Exception ex)
            {
                throw new Exception("将缓冲数组传递到为对应数据缓冲区时发生错误。\r\n" + ex.Message);
            }
            finally
            {
                //释放申请的内存
                Marshal.FreeHGlobal(buffer);
            }
        }

        //将两位十六进制数转换为对应字符串
        public string HexToString(int check)
        {
            string[] let = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", };
            string outs = "";
            int i = 0, j = 0;
            Byte Num = 0;

            Num = (Byte)(check & 0xff);

            i = Num >> 4;                 //储存高四位

            if (i == 0)                     //储存低四位
                j = Num;
            else
                j = Num - (i << 4);
            outs = let[i] + let[j];

            return outs;
        }

        //将以16进制格式书写的字符串处理成同义字符数组。
        //这些数字可以以空格中英文逗号以及"0x"和"0X"分割。
        //注意，团在一起的数字将被视为一个数，例如字符串"AB 98CDEFAB"将会被识别成数字0xab,0x98cdefab。
        public byte[] HexString2ByteArray(string data)
        {
            //处理数字转换
            string sendBuf = data;

            sendBuf = sendBuf.Replace(',', ' ');          //去掉英文逗号，以空格替换
            sendBuf = sendBuf.Replace('，', ' ');         //去掉中文逗号，以空格替换
            sendBuf = sendBuf.Replace("0x", "");          //去掉0x
            sendBuf = sendBuf.Replace("0X", "");          //去掉0X

            //按照空格间隔的方式将待发送的字符串分割
            string[] strArray = sendBuf.Split(' ');

            int byteBufferLength = strArray.Length;

            //计算有效的字符串数量
            for (int i = 0; i < strArray.Length; i++)
            {
                if (strArray[i] == "")
                {
                    byteBufferLength--;
                }
                else
                {
                    //不足偶数个字符的，在左侧补一个零字符位
                    if (strArray[i].Length % 2 != 0)
                    {
                        strArray[i] = strArray[i].PadLeft(strArray[i].Length + 1, '0');
                    }

                    //默认的是一个字符串对应一个字节
                    //此处考虑的是当一个字符串表述的内容超出一个字节时的情况
                    //即计算在字节缓冲区里面应该补充几个字节的位置
                    byteBufferLength += (strArray[i].Length / 2) - 1;
                }
            }

            //处理结果字符缓冲区
            byte[] byteBuffer = new byte[byteBufferLength];

            //字符缓冲区的赋值计数器
            int byteBufferCnt = 0;

            //遍历整个字符串数组
            for (int i = 0; i < strArray.Length; i++)        //对获取的字符做相加运算
            {
                if (strArray[i] == "")
                {
                    continue;
                }
                else
                {
                    //表征一个字节数值的字符串
                    string NumStr = "";

                    //这里默认使用小端存储模式（高字节存在地址高位），因为实测PC和STM32都是此种模式，以后有需要的话再做区分。
                    for (int j = (strArray[i].Length / 2) - 1; j >= 0; j--)
                    {
                        NumStr = strArray[i].Substring(j * 2, 2);

                        //将指定基数为16（进制）的数字的字符串表示形式转换为等效的 8 位无符号整数。
                        byteBuffer[byteBufferCnt] = Convert.ToByte(NumStr, 16);

                        byteBufferCnt++;
                    }
                }
            }
            return byteBuffer;
        }

        //将以字符格式书写的字符串处理成同义字符数组
        public byte[] String2ByteArray(string data)
        {
            byte[] byteBuffer = new byte[data.Length];
            char[] CharBuffer = data.ToCharArray();

            for (int i = 0; i < data.Length; i++)
            {
                byteBuffer[i] = Convert.ToByte(CharBuffer[i]);
            }
            
            return byteBuffer;
        }
        #endregion

        #region 数据信息获取方法

        //获取几个数中较大的值(此处应用了不定参数表,即参数可以为任意个同类数据)
        public int GetMaxValue(params int[] Values)
        {
            int result = 0;

            if (Values.Length == 0)
            {
                return 0;
            }

            result = Values[0];

            foreach (int Value in Values)
            {
                if (result < Value)
                {
                    result = Value;
                }
            }

            return result;
        }

        //获取某个类型变量的大小（所占字节数）
        public int GetSize(Type T)
        {
            return Marshal.SizeOf(T);
        }

        //获取几个变量类型中所占的最大的内存空间
        public int GetMaxSize(params Type[] Types)
        {
            int[] Length = new int[Types.Length];

            //获取各个类型的大小
            for (int i = 0; i < Types.Length; i++)
            {
                Length[i] = Marshal.SizeOf(Types[i]);
            }

            return GetMaxValue(Length);
        }

        //取一个短整型变量的各个字节段的数据
        public Byte GetByte(short Num, int Location)
        {
            short temp = Num;
            Byte High = (Byte)(temp >> 8);               //高八位
            Byte Low;                                    //低八位
            if (High >> 8 == 0)
                Low = (Byte)temp;
            else
                Low = (Byte)(temp - (short)High << 8);

            if (Location == 1)                   //获取高位数据
            {
                return High;
            }
            else if (Location == 0)              //获取低位数据
            {
                return Low;
            }
            return 0;

        }
        #endregion

        #region 数据相关的运算方法

        //计算校验值
        //此处使用求和校验,第2、3个参数指定计算校验的字节范围，返回值为uint。
        //以后可以扩展多种校验方式，使用输入参数指定校验类型。
        public uint GetCheckValue(byte[] DataBuffer, uint StartIndex, uint CheckLength)
        {
            uint checkValue = 0;

            //检测输入参数是否非法
            if((StartIndex + CheckLength) > (DataBuffer.Length - 1))
            {
                throw new Exception("<DataTool>:计算校验值时输入参数非法！");
            }

            for (uint i = StartIndex; i < StartIndex + CheckLength; i++)
            {
                checkValue += DataBuffer[i];
            }

            return checkValue;
        }
        #endregion
    }
}
