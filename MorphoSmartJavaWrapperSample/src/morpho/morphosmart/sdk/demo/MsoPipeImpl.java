package morpho.morphosmart.sdk.demo;

import java.util.ArrayList;

import com.morpho.morphosmart.pipe.Holder;
import com.morpho.morphosmart.pipe.IDeviceEnum;
import com.morpho.morphosmart.pipe.ILog;
import com.morpho.morphosmart.pipe.IMsoPipeConstants;
import com.morpho.morphosmart.pipe.PipeFunctions;

import morpho.morphosmart.sdk.api.IMsoPipe;


public class MsoPipeImpl implements IMsoPipe, IMsoPipeConstants, IDeviceEnum {
	
    private PipeFunctions functions = new PipeFunctions();
    private static IMsoPipe instance = null;
    private String server_ip;
    private int server_port;
    private ILog logger = null;
    
    private String serial;
	private int timeout;
    
    public MsoPipeImpl setLogger(ILog logger) {
		this.logger = logger;
		return this;
	}
    
    public void log(String msg) {
    	if(logger != null)
    		logger.log("MsoPipeImpl : " + msg + "\n");
    }

    public synchronized static IMsoPipe getInstance() {
        if(instance == null) {
            instance = new MsoPipeImpl();
        }
        return instance;
    }

    public MsoPipeImpl() {
    	server_ip = "127.0.0.1";
    	server_port = 11011;
    	instance = this;
    }
    
    @Override
	public String getSerial() {
		return this.serial;
	}

	@Override
	public int getTimeout() {
		return timeout;
	}

	
	
	@Override
	public void setSerial(String arg0) {
		this.serial = arg0;
		
	}

	@Override
	public void setTimeout(int arg0) {
		this.timeout = arg0;
	}
    
    public MsoPipeImpl setServer_ip(String server_ip) {
		this.server_ip = server_ip;
		return this;
	}
    public String getServer_ip() {
		return server_ip;
	}
    public MsoPipeImpl setServer_port(int server_port) {
		this.server_port = server_port;
		return this;
	}
    public int getServer_port() {
		return server_port;
	}

    @Override
    public int clientPipe_CallbackComOpen(Object o, String ip, long port, int timeout, String m_auc_MSOSerialNumber) {

    	log("[INF] Start clientPipe_CallbackComOpen " + timeout);
        int ret = 0;
        byte tag;

        ret = functions.TCP_Client_Connect(ip,(int)port);
        if ( ret == 0 )
        {
            tag = SP_PIPE_TAG_CONNECT_MSO;
            ret = functions.TCP_Send ( new byte[] { tag } );
        } else {
        	log("[ERR] TCP_Client_Connect return " + ret);
        }
        
        if ( ret == 0 )
        {
        	log("[INF] return 002 == " + m_auc_MSOSerialNumber);
            byte[] tmp = m_auc_MSOSerialNumber.getBytes();
            
            log("[INF] m_auc_MSOSerialNumber length " + tmp.length);
            
            byte[] sn;
            if(tmp[tmp.length-1] != '\0') {
                sn = new byte[tmp.length+1];
                for(int i = 0 ; i < tmp.length; i++) {
                    sn[i] = tmp[i];
                }
                sn[tmp.length] = '\0';
            } else {
                sn = tmp;
            }

            
            log("[INF] Sp_Pipe_Send_LenAndDatas sn.length " + sn.length);
            
            ret = functions.Sp_Pipe_Send_LenAndDatas ( sn );
        } else {
        	log("[ERR] TCP_Send SP_PIPE_TAG_CONNECT_MSO Error " + ret);
        }

        if ( ret == 0 )
        {
            Holder<byte[]> holder = new Holder<byte[]>();

            ret = functions.Sp_Pipe_Receive_Responses ( holder, SP_PIPE_TAG_CONNECT_MSO, timeout );
            
            if(ret != 0) {
            	log("[ERR] Sp_Pipe_Receive_Responses " + ret);
            }
            
        } else {
        	log("[ERR] Sp_Pipe_Send_LenAndDatas " + ret);
        }
        

        return ret;
    }

    @Override
    public int clientPipe_CallbackComSend(Object o, long timeout, ArrayList<Byte> arrayList) {
        log("[INF] ClientPipe_CallbackComSend sending: " + arrayList.size() + " to server timeout " + timeout );



        // Tag transmission (1 bytes)
        byte l_c_Tag = SP_PIPE_TAG_COMSEND;
        int l_i_Ret = functions.TCP_Send ( new byte[] { l_c_Tag } );

        // Len transmission (4 bytes)
        if ( l_i_Ret == 0 ) {

            byte[] bytes = functions.reverseInt(arrayList.size());

            l_i_Ret = functions.TCP_Send ( bytes );
            
            if(l_i_Ret != 0)
            	log("[ERR] TCP_Send length of data arrayList.size() " + arrayList.size() +  " l_i_Ret " + l_i_Ret);
            
        } else {
        	log("[ERR] TCP_Send SP_PIPE_TAG_COMSEND " + l_i_Ret );
        }

        if ( l_i_Ret == 0 ) {

            // prepare data
            byte[] data = new byte[arrayList.size()];
            for(int i = 0 ; i < data.length ; i++) {
                data[i] = arrayList.get(i).byteValue();
            }

            // Data transmission (Len bytes)
            l_i_Ret = functions.TCP_Send ( data );
            
            if(l_i_Ret != 0) {
            	log("[ERR] TCP_Send data error " + l_i_Ret);
            }
            
        }
        
        if ( l_i_Ret == 0 ) {
            Holder<byte[]> holder = new Holder<byte[]>();
            l_i_Ret = functions.Sp_Pipe_Receive_Responses ( holder, SP_PIPE_TAG_COMSEND, (int)timeout );
            
            if(l_i_Ret != 0) 
            	log("[ERR] Sp_Pipe_Receive_Responses error " + l_i_Ret);
        }

        return l_i_Ret;
    }

    @Override
    public int clientPipe_CallbackComReceive(Object o, long timeout, ArrayList<Byte> arrayList) {
        //log("[INF] clientPipe_CallbackComReceive timeout : " + timeout);
        
        Holder<byte[]> holder = new Holder<byte[]>();
        
        int l_i_Ret = functions.Sp_Pipe_Receive_Responses ( holder, SP_PIPE_TAG_COMRECV , (int)timeout );
        
        if(l_i_Ret == 0 && holder.data != null) {
            int count = holder.data.length;
            
            log("[INF] Sp_Pipe_Receive_Responses received " + count + " bytes");
            
            for(int i = 0 ; i < count ; i++) {
                arrayList.add(holder.data[i]);
            }
            
        } else {
            if(l_i_Ret != -10000)
        	    log("[ERR] Sp_Pipe_Receive_Responses " + l_i_Ret);
        }
        
        log("[INF] clientPipe_CallbackComReceive return " + l_i_Ret + " ");

        return l_i_Ret;
    }

    @Override
    public int clientPipe_CallbackComReceiveFree(Object o, ArrayList<Byte> arrayList) {
        log("[INF] clientPipe_CallbackComReceiveFree");
        return 0;
    }

    @Override
    public int clientPipe_CallbackComClose(Object o) {
    	log("[INF] clientPipe_CallbackComClose");
    	int l_i_Ret = functions.TCP_Client_Close ( );
    	log("[INF] TCP_Client_Close " + l_i_Ret);
        return l_i_Ret;
    }

	@Override
	public int enumerate(ArrayList<String> devices) {
		log("[INF] enumerate ");
		
		byte[] tag_to_receive = new byte[1];
        int l_c_NbUsbDevice = 0;

        log("[INF] Establishes a connection to a Server, specified by its Address, and it's Port Number " + server_ip + ":" + server_port);
        // Establishes a connection to a Server, specified by its Address, and it's Port Number
        int l_i_Ret = functions.TCP_Client_Connect ( server_ip, server_port);
        
        log("[INF] TCP_Client_Connect l_i_Ret " + l_i_Ret);
        
        if ( l_i_Ret == 0 ) {
            byte l_c_Tag = SP_PIPE_TAG_MSO_LIST;
            l_i_Ret = functions.TCP_Send ( new byte[] { l_c_Tag } );
            
            if(l_i_Ret != 0) {
            	log("[ERR] TCP_Send SP_PIPE_TAG_MSO_LIST " + l_i_Ret);
            }
            
        } else {
        	log("[ERR] TCP_Client_Connect " + l_i_Ret);
        }

        if ( l_i_Ret == 0 ) {
            l_i_Ret = functions.TCP_Recv ( tag_to_receive, tag_to_receive.length , 7000 );
        } else {
        	log("[ERR] TCP_Recv " + l_i_Ret);
        }

        if ( tag_to_receive[0] == SP_PIPE_TAG_MSO_LIST ) {
            
        	if ( l_i_Ret == 0 ) {
                l_i_Ret = functions.Sp_Pipe_Receive_Status (  );
            } else {
                l_i_Ret = -1;
            }

            if ( l_i_Ret == 0 ) {
                l_i_Ret = functions.TCP_Recv ( tag_to_receive, tag_to_receive.length,  7000 );
                l_c_NbUsbDevice = tag_to_receive[0];
            }

            if ( l_i_Ret == 0 ) {

                Holder<byte[]> holder = new Holder<byte[]>();

                for ( int i =0; i < l_c_NbUsbDevice; i++) {
                    holder.data = null;
                    l_i_Ret = functions.Sp_Pipe_Receive_LenAndDatas ( holder );
                    if(holder.data != null) {
                        String serial = new String(holder.data);
                        log("[INF] Device Serial " + serial);
                        devices.add(serial);
                    }
                }
            }
        }

        functions.TCP_Client_Close();

        return l_c_NbUsbDevice;
	}

	
}

