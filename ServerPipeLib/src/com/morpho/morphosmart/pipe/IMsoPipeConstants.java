package com.morpho.morphosmart.pipe;

/**
 * Created by GT909287 on 20/11/2017.
 */

public interface IMsoPipeConstants {
    public static final byte SP_PIPE_TAG_MSO_LIST = 'L'; // 76
    public static final byte SP_PIPE_TAG_CONNECT_MSO = 'C'; // 67
    public static final byte SP_PIPE_TAG_COMSEND = 'S'; // 83
    public static final byte SP_PIPE_TAG_COMRECV = 'R'; // 82
    
    public static final int MAX_RECV_TIME = 10000;
}
