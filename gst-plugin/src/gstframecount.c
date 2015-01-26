/*
 * GStreamer
 * Copyright (C) 2006 Stefan Kost <ensonic@users.sf.net>
 * Copyright (C) 2015 Paul Milliken <<user@hostname.org>>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-frame_count
 *
 * FIXME:Describe frame_count here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! frame_count ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <gst/controller/gstcontroller.h>

#include "gstframecount.h"

GST_DEBUG_CATEGORY_STATIC (gst_framecount_debug);
#define GST_CAT_DEFAULT gst_framecount_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT,
  PROP_FRAME_COUNT
};

/* the capabilities of the inputs and outputs.
 *
 * FIXME:describe the real formats here.
 */
static GstStaticPadTemplate sink_template =
GST_STATIC_PAD_TEMPLATE (
  "sink",
  GST_PAD_SINK,
  GST_PAD_ALWAYS,
  GST_STATIC_CAPS ("ANY")
);

static GstStaticPadTemplate src_template =
GST_STATIC_PAD_TEMPLATE (
  "src",
  GST_PAD_SRC,
  GST_PAD_ALWAYS,
  GST_STATIC_CAPS ("ANY")
);

/* debug category for fltering log messages
 *
 * FIXME:exchange the string 'Template frame_count' with your description
 */
#define DEBUG_INIT(bla) \
  GST_DEBUG_CATEGORY_INIT (gst_framecount_debug, "frame_count", 0, "Template frame_count");

GST_BOILERPLATE_FULL (Gstframecount, gst_framecount, GstBaseTransform,
    GST_TYPE_BASE_TRANSFORM, DEBUG_INIT);

static void gst_framecount_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_framecount_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static GstFlowReturn gst_framecount_transform_ip (GstBaseTransform * base,
    GstBuffer * outbuf);

/* GObject vmethod implementations */

static void
gst_framecount_base_init (gpointer klass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  gst_element_class_set_details_simple (element_class,
    "framecount",
    "Generic/Filter",
    "Number of frames since started",
    "Paul Milliken <<user@hostname.org>>");

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&src_template));
  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&sink_template));
}

/* initialize the frame_count's class */
static void
gst_framecount_class_init (GstframecountClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) klass;
  gobject_class->set_property = gst_framecount_set_property;
  gobject_class->get_property = gst_framecount_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
    g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE | GST_PARAM_CONTROLLABLE));

  g_object_class_install_property (gobject_class, PROP_FRAME_COUNT,
    g_param_spec_int ("frame_count", "Frame count", "Number of frames since stream started",
          0, G_PARAM_READWRITE | GST_PARAM_CONTROLLABLE));

  GST_BASE_TRANSFORM_CLASS (klass)->transform_ip =
      GST_DEBUG_FUNCPTR (gst_framecount_transform_ip);
}

/* initialize the new element
 * initialize instance structure
 */
static void
gst_framecount_init (Gstframecount *filter, GstframecountClass * klass)
{
  filter->silent = FALSE;
  filter->frame_count = 0;
}

static void
gst_framecount_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  Gstframecount *filter = GST_FRAME_COUNT (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
    case PROP_FRAME_COUNT:
      filter->frame_count = g_value_get_int (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_framecount_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  Gstframecount *filter = GST_FRAME_COUNT (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    case PROP_FRAME_COUNT:
      g_value_set_int (value, filter->frame_count);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstBaseTransform vmethod implementations */

/* this function does the actual processing
 */
static GstFlowReturn
gst_framecount_transform_ip (GstBaseTransform * base, GstBuffer * outbuf)
{
  Gstframecount *filter = GST_FRAME_COUNT (base);

  if (GST_CLOCK_TIME_IS_VALID (GST_BUFFER_TIMESTAMP (outbuf)))
    gst_object_sync_values (G_OBJECT (filter), GST_BUFFER_TIMESTAMP (outbuf));

  if (filter->silent == FALSE)
    g_print ("I'm plugged, therefore I'm in.\n");
  
  /* FIXME: do something interesting here.  This simply copies the source
   * to the destination. */
  /* Increment frame_count */
  filter->frame_count++;

  return GST_FLOW_OK;
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
frame_count_init (GstPlugin * frame_count)
{
  /* initialize gst controller library */
  gst_controller_init(NULL, NULL);

  return gst_element_register (frame_count, "frame_count", GST_RANK_NONE,
      GST_TYPE_FRAME_COUNT);
}

/* gstreamer looks for this structure to register frame_counts
 *
 * FIXME:exchange the string 'Template frame_count' with you frame_count description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "frame_count",
    "Number of frames since pipeline was started",
    frame_count_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
)
