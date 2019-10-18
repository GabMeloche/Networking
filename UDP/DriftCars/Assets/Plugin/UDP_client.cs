using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class UDP_client : MonoBehaviour
{
    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RunClient")]
    private static extern IntPtr RunClient();

    [DllImport("UDP", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Send")]
    private static extern void Send(IntPtr p_client, float p_x, float p_y, float p_z);

    private IntPtr m_client;
    // Start is called before the first frame update
    void Start()
    {
        m_client = RunClient();
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 pos = gameObject.transform.position;
        Send(m_client, pos.x, pos.y, pos.z);
    }
}
