#include "davix_tool_params.hpp"
#include <getopt.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>

namespace Davix{

namespace Tool{


const std::string scope_params = "Davix::Tools::Params";


#define CAPATH_OPT  1000
#define DEBUG_OPT  1001

OptParams::OptParams() :
    params(),
    vec_arg(),
    verbose(false),
    debug(false),
    req_type(),
    header_args(),
    help_msg(),
    cred_path()
{

}


static void option_abort(char** argv){
    std::cerr << argv[0] <<", Error: Wrong argument format" << std::endl;
    std::cerr << "try 'davix --help' for more informations" <<std::endl;
    exit(-1);
}

static void display_version(){
    std::cout << "Davix Version: " << version() << std::endl;
}


static int set_header_field(const std::string & arg, OptParams & p, DavixError** err){
    size_t pos;
    if( (pos = arg.find(':') ) == std::string::npos){
        DavixError::setupError(err, scope_params, StatusCode::InvalidArgument, " Invalid header field argument");
        return -1;
    }
    p.header_args.push_back(HeaderParam(arg.substr(0,pos), arg.substr(pos+1)));
    return 0;
}



int parse_davix_options_generic(const std::string &opt_filter,
                                const struct option* long_options,
                                int argc, char** argv, OptParams & p, DavixError** err){
    int ret =0;
    int option_index=0;

    while( (ret =  getopt_long(argc, argv, opt_filter.c_str(),
                               long_options, &option_index)) > 0){

        switch(ret){
            case 0:
              /* If this option set a flag, do nothing else now. */
              std::cout << " case 0 " << std::endl;
              break;
            case DEBUG_OPT:
                p.debug = true;
                davix_set_log_level(DAVIX_LOG_ALL);
                break;
            case 'E':
                 p.cred_path = std::string(optarg);
                 break;

            case 'k':
                p.params.setSSLCAcheck(false);
                break;
            case 'H':
                if( set_header_field(optarg, p, err) <0)
                    return -1;
                break;
            case CAPATH_OPT:
                p.params.addCertificateAuthorityPath(optarg);
                break;
            case 'V':
                display_version();
                return 1;

            case 'X':
                p.req_type = std::string(optarg, 0, 255);
            break;
            case '?':
            printf(p.help_msg.c_str(), argv[0]);
                return -1;
          break;
            default:
                option_abort(argv);
        }
    }


    if (optind < argc)
    {
        //std::cout << "  ARG " << argv[optind] << std::endl;
        p.vec_arg.push_back(argv[optind]);
        ret =0;
    }else{
        option_abort(argv);
    }

    return ret;
}



int parse_davix_options(int argc, char** argv, OptParams & p, DavixError** err){
    const std::string arg_tool_main= "H:E:X:M:vdkV";
    const struct option long_options[] = {
        {"header",  required_argument, 0,  'H' },
        {"cert",  no_argument,       0, 'E' },
        {"request",  required_argument, 0,  'X' },
        {"capath",  required_argument, 0, CAPATH_OPT },
        {"verbose", no_argument, 0,  0 },
        {"debug", no_argument, 0,  DEBUG_OPT },
        {"insecure", no_argument, 0,  'k' },
        {"Version", no_argument, 0, 'V'},
        {"help", no_argument,0,'?'},
        {0,         0,                 0,  0 }
     };

    return parse_davix_options_generic(arg_tool_main, long_options,
                                       argc, argv,
                                       p, err);
}


int parse_davix_ls_options(int argc, char** argv, OptParams & p, DavixError** err){
    const std::string arg_tool_main= "E:M:vdkV";
    const struct option long_options[] = {
        {"cert",  no_argument,       0, 'E' },
        {"capath",  required_argument, 0, CAPATH_OPT },
        {"verbose", no_argument, 0,  0 },
        {"debug", no_argument, 0,  DEBUG_OPT },
        {"insecure", no_argument, 0,  'k' },
        {"Version", no_argument, 0, 'V'},
        {"help", no_argument,0,'?'},
        {0,         0,                 0,  0 }
     };

    return parse_davix_options_generic(arg_tool_main, long_options,
                                       argc, argv,
                                       p, err);
}


}

}
