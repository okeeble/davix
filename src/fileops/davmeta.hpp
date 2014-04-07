/*
 * This File is part of Davix, The IO library for HTTP based protocols
 * Copyright (C) 2013  Adrien Devresse <adrien.devresse@cern.ch>, CERN
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
*/

#ifndef DAVMETA_HPP
#define DAVMETA_HPP

#include <davixcontext.hpp>
#include <params/davixrequestparams.hpp>
#include <file/davfile.hpp>

namespace Davix{

namespace Meta{


// get all reps from webdav queries
void getReplicas(Context & c, const Uri & r,
                              const RequestParams & params,  std::vector<DavFile> & vec);

dav_ssize_t posixStat(Context & c, const Uri & url, const RequestParams * _params,
                      struct stat* st,
                      DavixError** err);


int deleteResource(Context & c, const Uri & u, const RequestParams & params, DavixError** err);


int makeCollection(Context & c, const Uri & uri, const RequestParams & params, DavixError** err);


int checksum(Context & c, const Uri & uri, const RequestParams *params, std::string & checksm, const std::string & chk_algo);

/*
  retrieve a webdav propfind stat request to the given url
    @param req : http request where to executethe query
    @return vector of characters of the query content
  */
const char* req_webdav_propfind(HttpRequest* req, DavixError** err);


// utilities
int davix_metalink_header_parser(const std::string & header_key, const std::string & header_value,
                                 const Uri & u_original,
                                 Uri & metalink);


} // Meta

} // Davix




#endif // DAVMETA_HPP
