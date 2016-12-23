#ifndef MM7DEFINE_H_
#define MM7DEFINE_H_

/*

POST /mms-rs/mm7 HTTP/1.1
Host:mms.omms.com
Content-Type:multipart/related;boundary="NextPart_000_0028_01C19839.84698430";type=text/xml;
       start="</tnn-200102/mm7-submit>"
Content-Length:nnnn
SOAPAction:“"

--NextPart_000_0028_01C19839.84698430
Content-Type:text/xml;charset="utf-8"
Content-ID:</tnn-200102/mm7-submit>

<?xmlversion='1.0'?>
<env:Envelope xmlns:env="http://schemas.xmlsoap.org/soap/soap-envelope">
<env:Header>
      <mm7:TransactionID xmlns:mm7="http://www.3gpp.org/ftp/Specs/archive/23_series/23.140/schema/REL-5-MM7-1-0" env:mustUnderstand="1">
                    vas00001-sub
             </mm7:TransactionID>
   </env:Header>
       <env:Body>
       <mm7:SubmitReq xmlns:mm7="http://www.3gpp.org/ftp/Specs/archive/23_series/23.140/schema/REL-5-MM7-1-0">
                    <MM7Version>5.3.0</MM7Version>
                    <SenderIdentification>
                           <VASPID>TNN</VASPID>
                           <VASID>News</VASID>
                    </SenderIdentification>
                    <Recipients>
                           <To>
                                 <Number>7255441234</Number>
                                 <RFC2822AddressdisplayOnly="True">7255442222@OMMS.com</RFC2822Address>
                           </To>
                           <Cc>
                                 <Number>7255443333</Number>
                           </Cc>
                           <Bcc>
                                 <RFC2822Address>7255444444@OMMS.com</RFC2822Address>
                           </Bcc>
                    </Recipients>
                    <ServiceCode>gold-sp33-im42</ServiceCode>
                    <LinkedID>mms00016666</LinkedID>
                    <MessageClass>Informational</MessageClass>
                    <TimeStamp>2002-01-02T09:30:47-05:00</Date>
                    <EarliestDeliveryTime>2002-01-02T09:30:47-05:00</EarliestDeliveryTime>
                    <ExpiryDate>P90D</ExpiryDate>
                    <DeliveryReport>True</DeliveryReport>
                    <Priority>Normal</Priority>
                    <ChargedParty>Sender</ChargedParty>
                    <DistributionIndicator>True</DistributionIndicator>
                    <Subject>Newsfortoday</Subject>
                    <Contenthref="cid:SaturnPics-01020930@news.tnn.com";allowAdaptations="True"/>
             </mm7:SubmitReq>
   </env:Body>
</env:Envelope>

--NextPart_000_0028_01C19839.84698430
Content-Type:multipart/mixed;boundary="StoryParts7452684322002-77645"
Content-ID:<SaturnPics-01020930@news.tnn.com>

--StoryParts7452684322002-77645
Content-Type:text/plain;charset="us-ascii"

Sciencenews,newSaturnpictures...

--StoryParts7452684322002-77645
Content-Type:image/gif;
Content-ID:<saturn.gif>

R0lGODdhZAAwAOMAAAAAAIGJjGltcDE0OOfWo6Ochbi1n1pmcbGojpKbnP/lpW54fBMTE1RYXEFO
…

--StoryParts7452684322002-77645--
--_NextPart_000_0028_01C19839.84698430--
*/

enum EnumCMDType {
    CMD_SUBMIT_REQ,
    CMD_DELIVER_REQ,
    CMD_DELIVER_REPORT_REQ,
    CMD_READ_READY_REQ,
    CMD_SUBMIT_ACK,
    CMD_DELIVER_ACK,
    CMD_DELIVER_REPORT_ACK,
    CMD_READ_READY_ACK,
};

struct tagSoapMsg {
    uint32_t    length;
    uint32_t    cmdType;
    char        transactionID[120];
};

// 主送，抄送，密送
enum RECIPIENT_TYPE {
    REC_TO = 0,
    REC_CC = 1,
    REC_BCC = 2,
};

struct Recipient {
    RECIPIENT_TYPE recType;
    char phoneNumber[22];
    char rfc2822Address[60];
};

// 继承在sunOS系统有问题，好在不需要支持它
struct tagSoapSubmit : public tagSoapMsg {
    char vaspid[50];
    char vasid[50];
    char serviceCode[120];
    char linkID[120];
    char timeStamp[30];
    char earliestDeliveryTime[30];
    char expiryDate[10];
    uint8_t needReport;  // True/False
    char subject[140];

    uint16_t  receiverCount;
    Recipient  recipients[1]; // 变长数组
};

struct tagSoapDeliver : public tagSoapMsg {

};

#endif /* MM7DEFINE_H_ */
