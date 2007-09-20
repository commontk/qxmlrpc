#include <QtTest/QtTest>

#include "variant.h"
#include "request.h"
#include "response.h"

using namespace xmlrpc;

class TestQXmlRPC: public QObject
{
Q_OBJECT
private slots:
    void variant_data()
    {
        QTest::addColumn<QString>("xmlString");
        QTest::addColumn<Variant>("value");

        QTest::newRow("int") << "<value><int>21</int></value>" << Variant(21);
        QTest::newRow("i4") << "<value><i4>25</i4></value>" << Variant(25);
        QTest::newRow("boolean") << "<value><boolean>1</boolean></value>" << Variant(true);
        QTest::newRow("string") << "<value><string>test data</string></value>" << Variant( QString("test data") );
        QTest::newRow("string2") << "<value>test data</value>" << Variant( QString("test data") );
        QTest::newRow("base64") << "<value><base64>MTIzCg==</base64></value>" << Variant( QByteArray("123\n") );
        QTest::newRow("double") << "<value><double>1.0</double></value>" << Variant( 1.0 );
        QTest::newRow("empty array") << "<value><array><data></data></array></value>" << Variant( QList<Variant>() );

        QString testListString = "<value><array><data>"
                                  "<value><int>1</int></value>"
                                  "<value><int>2</int></value>"
                                  "</data></array></value>";
        
        QList<Variant> testList;
        testList << Variant(1);
        testList << Variant(2);

        QTest::newRow("array") << testListString << Variant( testList );

        QString testStructString = \
            "<value>"
            "  <struct>"
            "   <member>"
            "      <name>lowerBound</name>"
            "      <value><i4>18</i4></value>"
            "      </member>"
            "   <member>"
            "      <name>upperBound</name>"
            "      <value><i4>139</i4></value>"
            "      </member>"
            "  </struct>" 
            "</value>";

        QMap<QString, Variant> testStruct;
        testStruct["lowerBound"] = Variant(18);
        testStruct["upperBound"] = Variant(139);
        
        QTest::newRow("struct") << testStructString << Variant( testStruct );
    }

    void variant()
    {

        QFETCH(QString, xmlString);
        QFETCH(Variant, value);

        //test xml->variant conversion

        
        QDomDocument doc;

        QString domErrorMsg;
        int domErrorLine;
        int domErrorColumn;

        if ( !doc.setContent( xmlString, &domErrorMsg, &domErrorLine, &domErrorColumn) ) {
            qDebug() << domErrorMsg;
            qDebug() << "line:" << domErrorLine << "column:" << domErrorColumn;
            QFAIL("Failed parsing sample xml data");
        }

        Variant v( doc.firstChildElement() );


        if ( v != value ) {
            qDebug() << v;
            qDebug() << "expected value:" << value;
        }


        QCOMPARE( v, value );

        //test variant->xml->variant conversion
        QDomDocument doc2;
        QDomElement valueElement = v.toDomElement(doc2);
        Variant v2( valueElement );
        
        QCOMPARE( v, v2 );
        
    }


    void request() 
    {
        QString requestData = \
            "<methodCall>"
            "  <methodName>testCall</methodName>"
            "  <params>"
            "    <param>"
            "      <value>"
            "        <string>string parameter</string>"
            "      </value>"
            "    </param>"
            "    <param>"
            "      <value>"
            "        <int>12</int>"
            "      </value>"
            "    </param>"
            "  </params>"
            "</methodCall>";

        Request request;
        
        QString errorMsg;
        if ( !request.setContent( requestData.toUtf8(), &errorMsg) ) {
            qDebug() << errorMsg;
            QFAIL("Failed processing request data");
        }

        QCOMPARE( request.isNull(), false );

        QCOMPARE( request.methodName(), QString("testCall") );

        QList<Variant> expectedParams;
        expectedParams << Variant( QString("string parameter") );
        expectedParams << Variant( 12 );

        QCOMPARE( request.parameters(), expectedParams );

        // now test conversion to xml and back to data

        QByteArray composedRequest = request.composeRequest();
        Request request2;

        if ( !request2.setContent( composedRequest, &errorMsg) ) {
            qDebug() << errorMsg;
            QFAIL("Failed processing request data");
        }


        QCOMPARE( request.methodName(), request2.methodName() );
        QCOMPARE( request.parameters(), request2.parameters() );

        //test for incorrect request:

        // incorrect xml
        QCOMPARE( Request().setContent((requestData+"aaa").toUtf8()), false );
        
        QString illRequest = requestData;
        illRequest.replace("methodName","method_Name");
        QCOMPARE( Request().setContent( illRequest.toUtf8() ), false );




    }

    
    void response_data() 
    {
        QTest::addColumn<QString>("responseData");
        QTest::addColumn<bool>("isNull");
        QTest::addColumn<bool>("isFault");
        QTest::addColumn<int>("faultCode");
        QTest::addColumn<QString>("faultString");
        QTest::addColumn<Variant>("returnValue");


        QString responseData;

        responseData = \
            "<?xml version=\"1.0\"?>"
            "<methodResponse>"
            "   <fault>"
            "      <value>"
            "         <struct>"
            "            <member>"
            "               <name>faultCode</name>"
            "               <value><int>4</int></value>"
            "               </member>"
            "            <member>"
            "               <name>faultString</name>"
            "               <value><string>Too many parameters.</string></value>"
            "               </member>"
            "            </struct>"
            "         </value>"
            "      </fault>"
            "   </methodResponse>";

        QTest::newRow("fault") << responseData << false << true << 4 << QString("Too many parameters.") << Variant(0);

        responseData = \
            "<?xml version=\"1.0\"?>"
            "<methodResponse>"
            "   <params>"
            "      <param>"
            "         <value><string>South Dakota</string></value>"
            "         </param>"
            "      </params>"
            "</methodResponse>";
        
        QTest::newRow("return vaue") << responseData << false << false << 0 << QString() << Variant(QString("South Dakota"));


    }

    void response() 
    {
        QFETCH( QString, responseData );
        QFETCH( bool, isNull );
        QFETCH( bool, isFault );
        QFETCH( int, faultCode );
        QFETCH( QString, faultString );
        QFETCH( Variant, returnValue );
        
        Response response;
        
        QString errorMsg;
        if ( !response.setContent( responseData.toUtf8(), &errorMsg) ) {
            qDebug() << errorMsg;
            QFAIL("Failed processing response data");
        }

        QCOMPARE( response.isNull(), isNull );

        if ( !isNull ) {
        
            QCOMPARE( response.isFault(), isFault );

            if ( isFault ) {
                QCOMPARE( response.faultCode(), faultCode );
                QCOMPARE( response.faultString(), faultString );
            } else {
                QCOMPARE( response.returnValue(), returnValue );
            }


            // now encode this request, decode it again and compare the result:
            QByteArray composedResponse = response.composeResponse();
            Response response2;

            if ( !response2.setContent( composedResponse, &errorMsg) ) {
                qDebug() << errorMsg;
                QFAIL("Failed processing rerponse data");
            }

            QCOMPARE( response.isFault(), response2.isFault() );

            if ( isFault ) {
                QCOMPARE( response.faultCode(), response2.faultCode() );
                QCOMPARE( response.faultString(), response2.faultString() );
            } else {
                QCOMPARE( response.returnValue(), response2.returnValue() );
            }


        }



    }

    
};

Q_DECLARE_METATYPE( Variant )

