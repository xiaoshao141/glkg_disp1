#include "ctemprmtp.h"

#include <QDebug>

QImage g_aTempImg;
QImage g_bTempImg;
QImage g_cTempImg;

CTempRmtp::CTempRmtp()
{

}

CTempRmtp::CTempRmtp(QString camIp, qint8 camId)
{
    m_camIp = camIp;
    m_camId = camId;
}

void CTempRmtp::startRqstImg()
{
    this->start();
}

void CTempRmtp::run()
{
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    AVPacket *packet;
    uint8_t *out_buffer;

    struct SwsContext *img_convert_ctx;

    int videoStream,  numBytes;
    unsigned int i;
    int ret, got_picture;

    pFormatCtx = avformat_alloc_context();


    AVDictionary *avdic = NULL;

    av_dict_set(&avdic, "rtsp_transport", "tcp", 0);
    av_dict_set(&avdic, "max_delay", "100", 0);
    av_dict_set(&avdic, "buffer_size", "1024000", 0);
    av_dict_set(&avdic, "stimeout", "3000000", 0);

    QString url("rtsp://admin:ad53937301@");
    url += m_camIp;
    url += ":554/h264/ch2/sub/av_stream";

    if (avformat_open_input(&pFormatCtx, url.toLatin1(), NULL, &avdic) != 0) {
        qDebug() << "can't open the file.";
        return;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        qDebug() << "Could't find stream infomation.";
        return;
    }

    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }

    if (videoStream == -1) {
        qDebug() << "Didn't find a video stream.";
        return;
    }

    ///查找解码器
   pCodecCtx = pFormatCtx->streams[videoStream]->codec;
   pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

   pCodecCtx->bit_rate = 0;   //初始化为0
   pCodecCtx->time_base.num = 1;  //下面两行：一秒钟25帧
   pCodecCtx->time_base.den = 25;
   pCodecCtx->frame_number = 1;  //每包一个视频帧

   if (pCodec == NULL) {
       printf("Codec not found.\n");
       return;
   }

   ///打开解码器
   if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
       printf("Could not open codec.\n");
       return;
   }

   pFrame = av_frame_alloc();
   pFrameRGB = av_frame_alloc();

   ///这里我们改成了 将解码后的YUV数据转换成RGB32
   img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
           pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
           AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

   numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

   out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
   avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB32,
           pCodecCtx->width, pCodecCtx->height);

   int y_size = pCodecCtx->width * pCodecCtx->height;

   packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
   av_new_packet(packet, y_size); //分配packet的数据

   while (1)
   {
       if (av_read_frame(pFormatCtx, packet) < 0)
       {
           break; //这里认为视频读取完了
       }

       if (packet->stream_index == videoStream) {
           ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

           if (ret < 0) {
               printf("decode error.\n");
               return;
           }

           if (got_picture) {
               sws_scale(img_convert_ctx,
                       (uint8_t const * const *) pFrame->data,
                       pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                       pFrameRGB->linesize);
               //把这个RGB数据 用QImage加载
               QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);

               if (m_camId == 1)
               {
                   g_aTempImg = tmpImg.copy();
               }
               else if (m_camId == 2)
               {
                    g_bTempImg = tmpImg.copy();
               }
               else if (m_camId == 3)
               {
                    g_cTempImg = tmpImg.copy();
               }
           }

       }
       av_free_packet(packet); //释放资源,否则内存会一直上升

    ///2017.8.7---lizhen
       //msleep(0.02); //停一停  不然放的太快了
      QThread::msleep(1);
   }

   av_free(out_buffer);
   av_free(pFrameRGB);
   avcodec_close(pCodecCtx);
   avformat_close_input(&pFormatCtx);
}

void CTempRmtp::initFfmpeg()
{
    avformat_network_init();
    av_register_all();
}
