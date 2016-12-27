#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <fstream>
#include <algorithm>

#include <glog/logging.h>

#include "http_parser.h"
#include <libxml/xmlreader.h>


struct SoapParser {
    typedef http_parser_settings HttpParserSettings;

    HttpParserSettings http_settings;
    http_parser     httpParser;
    xmlTextReaderPtr pXmlReader;

    const char * contentType;
    size_t contentTypeLen;
};

static const char* CONTENT_TYPE = "Content-Type";


int on_body(http_parser* p, const char *at, size_t length) {
    LOG(INFO) << "i see body, len=" << length;

    xmlTextReaderPtr pReader =  xmlReaderForMemory( at, length, NULL, "utf-8", 0
            // TODO::
            // extract real soap xml position from html body
            );

    return 0;
}

int on_head_field(http_parser* p, const char *at, size_t length) {

    char buff[1024];
    memset(buff, 0, sizeof(buff));
    int v = std::min(sizeof(buff), length + 1);
    snprintf(buff, v, "%s", at);

    LOG(INFO)<< "field: " << buff;

    const char *needField = CONTENT_TYPE;
    if (0 == strncmp(at, needField, strlen(needField))) {

    }
    return 0;
}

int on_head_value(http_parser* p, const char *at, size_t length) {
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    int v = std::min(sizeof(buff), length + 1);
    snprintf(buff, v, "%s", at);

    LOG(WARNING)<< buff;
    return 0;
}

void readContent(const char * fpath, char ** fileContent, size_t &contentSize) {
    FILE* fp = fopen(fpath, "rb");
    char *file_data;

    if (fp) {
        fseek(fp, 0, SEEK_END);
        contentSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        file_data = (char*) malloc(contentSize);
        fread(file_data, 1, contentSize, fp);
        fclose(fp);
        LOG(INFO)<< "content has been read";
    } else {
        contentSize = 0;
    }
}

SoapParser * initSoapParser() {
    SoapParser * p = new SoapParser;
    memset(p, 0, sizeof(*p));

    p->http_settings.on_body = on_body;
    p->http_settings.on_header_field = on_head_field;
    p->http_settings.on_header_value = on_head_value;

    http_parser_init(& p->httpParser, HTTP_REQUEST);
    p->httpParser.data = p; // 把自己藏进去
    return p;
}

int main(int argc, char**argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;

    SoapParser * sp = initSoapParser();
    char * content;
    size_t size;

    readContent("soapmsg.bin", &content, size);
    LOG(INFO)<< "star to parse soap msg";
    size_t ret = http_parser_execute(&sp->httpParser, &sp->http_settings, content, size);
    if (ret != size) {
        LOG(INFO)<< "parsed = " << ret << ", real length = " << size;
        LOG(INFO)<< "http_parse: " << http_errno_name(HTTP_PARSER_ERRNO(&sp->httpParser)) << ", " << http_errno_description(HTTP_PARSER_ERRNO(&sp->httpParser));
    }

    return 0;
}
