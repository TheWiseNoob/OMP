/* ////////////////////////////////////////////////////////////////////////////   
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  The developer(s) of the OMP audio player hereby grant(s) permission
//  for non-GPL compatible GStreamer plugins to be used and distributed
//  together with GStreamer and OMP. This permission is above and beyond
//  the permissions granted by the GPL license by which OMP is covered.
//  If you modify this code, you may extend this exception to your version
//  of the code, but you are not obligated to do so. If you do not wish to do
//  so, delete this exception statement from your version.
//
//  Libraries used by OMP:
//
//    - clastfm: http://liblastfm.sourceforge.net/ 
//
//    - gstreamer: https://gstreamer.freedesktop.org/ 
//
//    - gtkmm: https://www.gtkmm.org/en/
//
//    - libconfig: http://www.hyperrealm.com/libconfig/
//
//    - standard C and C++ libraries
//
//    - taglib: http://taglib.org/
//
//////////////////////////////////////////////////////////////////////////// */





//         //
//         //
//         //
// Headers ////////////////////////////////////////////////////////////////////
//         //
//         //
//         //

//              //
//              //
// Class Header ///////////////////////////////////////////////////////////////
//              //
//              //

#include "TrackBin.h"





//                 //
//                 //
// Program Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include "../Base.h"

#include "../Metadata/Track.h"

#include "Playback.h"





//                 //
//                 //
// Outside Headers ////////////////////////////////////////////////////////////
//                 //
//                 //

#include <iostream>

#include <gst/gst.h>

#include <gtkmm/treerowreference.h>





//                 //
//                 //
//                 //
// Class Functions ////////////////////////////////////////////////////////////
//                 //
//                 //
//                 //

//             //
//             //
// Constructor ////////////////////////////////////////////////////////////////
//             //
//             //

TrackBin::TrackBin(Base& base, Track* new_track, 
                   GstElement* new_track_bin, 
                   Gtk::TreeRowReference new_track_row_ref,
                   GstElement* new_source, GstElement* new_parser, 
                   GstElement* new_decoder, GstElement* new_demuxer,
                   GstElement* new_conv, GstElement* new_queue)

// Inherited Class
: Parts(base)

// Bin Parts
, concat_request_sinkpad_(nullptr)
, conv_(new_conv)
, decoder_(new_decoder)
, demuxer_(new_demuxer)
, parser_(new_parser)
, queue_(new_queue)
, source_(new_source)
, track_bin_(new_track_bin)

// State Indicators
, seeked_(false)

// Track Data
, row_ref_(new Gtk::TreeRowReference(new_track_row_ref))
, track_(new_track)

{  

  // Checks if track_bin_ is nullptr or not!
  if(track_bin_ == nullptr)
  {

    debug("trackbin == nullptr");

  }

  // Checks if source_ is nullptr or not!
  else if(source_ == nullptr)
  {

    debug("source == nullptr");

  }

  // Checks if parser_ is nullptr or not!
  else if(parser_ == nullptr)
  {

    debug("parser == nullptr");

  }

  // Checks if decoder_ is nullptr or not!
  else if(decoder_ == nullptr)
  {

    debug("decoder == nullptr");

  }

  // Checks if conv_ is nullptr or not!
  else if(conv_ == nullptr)
  {

    debug("conv == nullptr");

  }

  // Checks if queue_ is nullptr or not!
  else if(queue_ == nullptr)
  {

    debug("queue == nullptr");

  }



  // Checks if the track row iterator is valid or not.
  if(!(*row_ref_))
  {

    debug("Row it in trackbin creation not valid!");

  } 

  // True if the track row it is invalid.
  else
  {

    debug("Row it in trackbin creation valid!");

  }

}





//            //
//            //
// Destructor /////////////////////////////////////////////////////////////////
//            //
//            //

TrackBin::~TrackBin()
{

  debug("Deleting TrackBin!");



  // Deletes the Track data structure from memeory.
  delete track_;



  // Sets the state of the track's GstBin to NULL.
  gst_element_set_state(track_bin_, GST_STATE_NULL);



  // 
  gst_object_unref(track_bin_);



  // Deletes the new-created track row iterator.
  delete row_ref_;

}





//                  //
//                  //
// Member Functions ///////////////////////////////////////////////////////////
//                  //
//                  //

//                       //
// Pipeline Manipulators //////////////////////////////////////////////////////
//                       //

void TrackBin::Attach()
{

  debug("Beginning of Attach");



  // Adds the track bin to the playback pipeline.
  gst_bin_add(GST_BIN(playback() . pipeline()), track_bin_);



  // The template for the request sinkpad of the concat.
  static GstStaticPadTemplate concat_static_request_sinkpad_template 
    = GST_STATIC_PAD_TEMPLATE("sink_%u", GST_PAD_SINK, GST_PAD_REQUEST,
                              GST_STATIC_CAPS("ANY"));



  debug("After concat_static_request_sinkpad_template creation!");



  // Converts the static request sinkpad to a nonstatic sinkpad.
  GstPadTemplate* concat_request_sinkpad_template 
    = gst_static_pad_template_get
        (&concat_static_request_sinkpad_template);



  debug("After static pad creation!");



  // Uses the new template to get a concat request sinkpad.
  concat_request_sinkpad_
    = gst_element_request_pad(playback().concat(), 
                              concat_request_sinkpad_template, NULL, NULL);



  debug("After concat_request_sinkpad creation!");



  // Gets the srcpad of the track bin.
  GstPad* track_bin_srcpad = gst_element_get_static_pad(track_bin_, "src");



  // Attaches the track bin's srcpad to the request sinkpad of concat.
  if(gst_pad_link(track_bin_srcpad, concat_request_sinkpad_) == GST_PAD_LINK_OK)
  {

    debug("track_bin_pad and concat_request_sinkpad linked!");

  }

  // True if the linking fails.
  else
  {

    debug("Linking of playing_trackbin_pad and concat_request_pad failed!");

  } 



  debug("After linking of playing_trackbin_pad and concat_request_pad!");




  // Unrefs the track bin's srcpad. 
  g_object_unref(track_bin_srcpad);

}

void TrackBin::Detach()
{

  // Unlinks the track bin from the concat element.
  gst_element_unlink(track_bin_, playback().concat());



  // Removes the track bin from the pipeline.
  gst_bin_remove((GstBin*)(playback().pipeline()), track_bin_);



  // Release the request concat request sinkpad.
  gst_element_release_request_pad
    (playback().concat(), concat_request_sinkpad_);

  // Unrefs the request sinkpad so it is destroyed.
  gst_object_unref(concat_request_sinkpad_);

}





//         //
//         //
// Getters ////////////////////////////////////////////////////////////////////
//         //
//         //

//           //
// Bin Parts //////////////////////////////////////////////////////////////////
//           //

GstElement* TrackBin::conv()
{

  return conv_;

}

GstElement* TrackBin::decoder()
{

  return decoder_;

}

GstElement* TrackBin::demuxer()
{

  return demuxer_;

}

GstElement* TrackBin::parser()
{

  return parser_;

}

GstElement* TrackBin::queue()
{

  return queue_;

}

GstElement* TrackBin::source()
{

  return source_;

}

GstElement* TrackBin::track_bin()
{ 

  return track_bin_; 

}





//                  //
// State Indicators ///////////////////////////////////////////////////////////
//                  //

std::atomic<bool>& TrackBin::seeked()
{ 

  return seeked_;

}





//            //
// Track Data /////////////////////////////////////////////////////////////////
//            //

Gtk::TreeRowReference& TrackBin::row_ref()
{

  return *row_ref_;

}

Track& TrackBin::track()
{ 

  return *track_; 

}
