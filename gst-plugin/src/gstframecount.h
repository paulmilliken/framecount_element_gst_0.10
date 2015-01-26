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
 
#ifndef __GST_FRAME_COUNT_H__
#define __GST_FRAME_COUNT_H__

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>

G_BEGIN_DECLS

#define GST_TYPE_FRAME_COUNT \
  (gst_framecount_get_type())
#define GST_FRAME_COUNT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_FRAME_COUNT,Gstframecount))
#define GST_FRAME_COUNT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_FRAME_COUNT,GstframecountClass))
#define GST_IS_FRAME_COUNT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_FRAME_COUNT))
#define GST_IS_FRAME_COUNT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_FRAME_COUNT))

#define MIN_FRAME_COUNT 0
#define MAX_FRAME_COUNT 2000000000
#define DEFAULT_FRAME_COUNT 0

typedef struct _Gstframecount      Gstframecount;
typedef struct _GstframecountClass GstframecountClass;

struct _Gstframecount {
  GstBaseTransform element;

  gboolean silent;
  gint frame_count;
};

struct _GstframecountClass {
  GstBaseTransformClass parent_class;
};

GType gst_framecount_get_type (void);

G_END_DECLS

#endif /* __GST_FRAME_COUNT_H__ */
