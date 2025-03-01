LIBS += -lusb-1.0
QT += opengl

INCLUDEPATH += $$PWD
HEADERS += $$PWD/we_correlator_hw.h

HEADERS += $$PWD/processing_chain.h
SOURCES += $$PWD/processing_chain.cpp
HEADERS += $$PWD/we_writer.h
HEADERS += $$PWD/worker_element.h
HEADERS += $$PWD/qqueue.h
HEADERS += $$PWD/qfifo.h
HEADERS += $$PWD/helper.h
HEADERS += $$PWD/frame.h
HEADERS += $$PWD/textcolor.h
HEADERS += $$PWD/lut.h
HEADERS += $$PWD/prng.h
HEADERS += $$PWD/worker_thread.h
HEADERS += $$PWD/worker_interface.h
HEADERS += $$PWD/we_frame_analyzer.h
HEADERS += $$PWD/we_sink_source.h
HEADERS += $$PWD/we_reader_raw.h
HEADERS += $$PWD/we_frame_gen.h
HEADERS += $$PWD/we_copy_nb.h
HEADERS += $$PWD/we_display.h
HEADERS += $$PWD/we_display2.h
HEADERS += $$PWD/we_accumulate.h
HEADERS += $$PWD/we_split.h
HEADERS += $$PWD/we_reader_usb_rh2.h
HEADERS += $$PWD/we_frame_gen_rnd.h
HEADERS += $$PWD/we_null.h
HEADERS += $$PWD/we_roi.h
HEADERS += $$PWD/we_correlator.h
HEADERS += $$PWD/we_bridge_outlet.h
HEADERS += $$PWD/we_display_graph.h
HEADERS += $$PWD/we_endpoint.h
HEADERS += $$PWD/we_frame_gen_chspad.h

SOURCES += $$PWD/worker_element.cpp
SOURCES += $$PWD/worker_thread.cpp
SOURCES += $$PWD/worker_interface.cpp

include(../yaidtools.pri)
