
#ifndef UTILS_TYPEDEFS_H_
#define UTILS_TYPEDEFS_H_

typedef union{
    unsigned int B;
    struct
    {
       unsigned int Button             :1;

       unsigned int UART_RX            :1;
       unsigned int UART_RX_cnt_LAST   :3;
       unsigned int UART_RX_cnt        :3;

       unsigned int GDO_0_Set          :1;
       unsigned int GDO_2_Set          :1;

       unsigned int Counter            :3;
       //unsigned int                   :3;
    };
}__Status_t;

typedef union{
    unsigned int B;
    struct
    {
       unsigned int ADC_Val         :10;
       unsigned int DataReady       :1;
       unsigned int channel         :4;
    };
}__ADC_Status_t;

typedef union{
    float f;
    unsigned int i[2];
    unsigned char b[4];
}__TypeCast;


#endif /* UTILS_TYPEDEFS_H_ */
